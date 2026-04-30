// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The Bitcoin Core developers
// Copyright (c) 2025-2026 Heiwabitnull (RXB Core)
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include <pow.h>
#include <arith_uint256.h>
#include <chain.h>
#include <chainparams.h>
#include <primitives/block.h>
#include <uint256.h>

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast,
                                       int64_t nFirstBlockTime,
                                       const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    int64_t nIdealTimespan  = params.nPowTargetTimespan;

    // Asymmetric clamp:
    // Max difficulty increase: 2% per retarget window (floor at 98% of ideal time)
    // Max difficulty decrease: 55% per retarget window (ceiling at 222% of ideal time)
    int64_t nMinTimespan = (nIdealTimespan * 98) / 100;
    int64_t nMaxTimespan = (nIdealTimespan * 222) / 100;

    if (nActualTimespan < nMinTimespan) nActualTimespan = nMinTimespan;
    if (nActualTimespan > nMaxTimespan) nActualTimespan = nMaxTimespan;

    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= nIdealTimespan;

    if (bnNew > bnPowLimit) bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast,
                                 const CBlockHeader* pblock,
                                 const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    // Only retarget once per adjustment interval (every 2016 blocks)
    if ((pindexLast->nHeight + 1) % params.DifficultyAdjustmentInterval() != 0)
        return pindexLast->nBits;

    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval() - 1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
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
