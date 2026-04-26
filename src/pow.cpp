#include <pow.h>
#include <arith_uint256.h>
#include <chain.h>
#include <consensus/consensus.h>
#include <chainparams.h>
#include <primitives/block.h>
#include <uint256.h>
#include <vector>
#include <algorithm>

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast,
                                 const CBlockHeader* pblock,
                                 const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    const int64_t N = 60;
    const int64_t T = params.nPowTargetSpacing;
    const int64_t k = N * (N + 1) / 2;
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);

    if (pindexLast->nHeight < N) {
        return bnPowLimit.GetCompact();
    }

    std::vector<const CBlockIndex*> blocks;
    blocks.reserve(N + 1);
    const CBlockIndex* pindex = pindexLast;
    for (int i = 0; i <= N; ++i) {
        blocks.push_back(pindex);
        pindex = pindex->pprev;
        if (!pindex) {
            return bnPowLimit.GetCompact();
        }
    }

    int64_t weighted_sum = 0;
    for (int i = 0; i < N; ++i) {
        int64_t solvetime = blocks[i]->GetBlockTime()
                          - blocks[i + 1]->GetBlockTime();

        solvetime = std::max<int64_t>(solvetime, 1);

        // Raised from 6*T to 100*T so the algorithm properly accounts
        // for extended periods of inactivity on the chain.
        solvetime = std::min<int64_t>(solvetime, 100 * T);

        weighted_sum += solvetime * (N - i);
    }

    int64_t LWMA = weighted_sum / k;
    if (LWMA < 1) LWMA = 1;

    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);

    if (T <= 0) return bnPowLimit.GetCompact();

    bnNew *= arith_uint256(LWMA);
    bnNew /= arith_uint256(T);

    arith_uint256 bnPrev;
    bnPrev.SetCompact(pindexLast->nBits);

    if (bnPrev > bnPowLimit) bnPrev = bnPowLimit;

    // Difficulty may increase by no more than 5 percent per block.
    // bnPrev * 95 is well within 256-bit bounds given standard powLimit.
    arith_uint256 bnMaxGrowth = (bnPrev * 95) / 100;

    // Difficulty may decrease by no more than 33 percent per block.
    // bnPrev * 150 is well within 256-bit bounds given standard powLimit.
    arith_uint256 bnMaxDrop = (bnPrev * 150) / 100;

    if (bnMaxDrop > bnPowLimit) bnMaxDrop = bnPowLimit;

    if (bnNew < bnMaxGrowth) bnNew = bnMaxGrowth;
    if (bnNew > bnMaxDrop)   bnNew = bnMaxDrop;

    if (bnNew == 0)          bnNew = 1;
    if (bnNew > bnPowLimit)  bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash,
                      unsigned int nBits,
                      const Consensus::Params& params)
{
    arith_uint256 bnTarget;
    bnTarget.SetCompact(nBits);

    if (bnTarget <= 0 || bnTarget > UintToArith256(params.powLimit))
        return false;

    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}
