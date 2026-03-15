#include <chainparams.h>
#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <tinyformat.h>
#include <util/system.h>
#include <util/strencodings.h>
#include <arith_uint256.h>
#include <versionbitsinfo.h>
#include <cassert>
#include <cinttypes>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <iostream>

static CBlock CreateGenesisBlock(
    const char* pszTimestamp,
    const CScript& genesisOutputScript,
    uint32_t nTime,
    uint32_t nNonce,
    uint32_t nBits,
    int32_t nVersion,
    const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig =
        CScript()
        << 486604799
        << CScriptNum(4)
        << std::vector<unsigned char>(
            (const unsigned char*)pszTimestamp,
            (const unsigned char*)pszTimestamp + strlen(pszTimestamp)
        );
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;
    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

static CBlock CreateGenesisBlock(
    uint32_t nTime,
    uint32_t nNonce,
    uint32_t nBits,
    int32_t nVersion,
    const CAmount& genesisReward)
{
    const char* pszTimestamp =
        "The Economist February 28th-March 6th 2026 Digging for Victory The daunting quest for critical minerals";

    const CScript genesisOutputScript =
        CScript()
        << OP_RETURN
        << ParseHex("5468652045636f6e6f6d69737420466562727561727920323874682d4d617263682036746820323032362044696767696e6720666f7220566963746f727920546865206461756e74696e6720717565737420666f7220637269746963616c206d696e6572616c73");

    return CreateGenesisBlock(
        pszTimestamp,
        genesisOutputScript,
        nTime,
        nNonce,
        nBits,
        nVersion,
        genesisReward
    );
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = CBaseChainParams::MAIN;
        consensus.nSubsidyHalvingInterval = 195000;
        consensus.BIP16Exception = uint256S("0x0");
        consensus.BIP34Height = 0;
        consensus.BIP34Hash   = uint256S("0x0");
        consensus.BIP65Height = 0;
        consensus.BIP66Height = 0;
        consensus.CSVHeight    = 0;
        consensus.SegwitHeight = 0;
        consensus.MinBIP9WarningHeight = 0;
        consensus.powLimit = uint256S("00000000ffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowTargetTimespan = 10 * 60;
        consensus.nPowTargetSpacing  = 10 * 60;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nMinerConfirmationWindow       = 144;
        consensus.nRuleChangeActivationThreshold = 108;
        consensus.defaultAssumeValid = uint256S("0x0");
        consensus.nMinimumChainWork  = uint256S("0x0");

        pchMessageStart[0] = 0xD9;
        pchMessageStart[1] = 0x51;
        pchMessageStart[2] = 0xAE;
        pchMessageStart[3] = 0x87;
        nDefaultPort = 8327;
        nPruneAfterHeight = 100000;
        m_assumed_blockchain_size  = 1;
        m_assumed_chain_state_size = 1;

        genesis = CreateGenesisBlock(
            1772313120,
            3775284435,
            0x1d00ffff,
            1,
            100 * COIN
        );

        consensus.hashGenesisBlock = uint256S("0x0000000014766203f48e47b0c76ac24c9c23a1c9aeacce12c4e3648194f68b50");
        assert(genesis.hashMerkleRoot == uint256S("0x2f41a21f88d745bdeeff375226e5a8b676381157cdd8106459a1cf27a194bc10"));
        assert(genesis.GetHash() == consensus.hashGenesisBlock);

        vSeeds.clear();
        vSeeds.emplace_back("5v2v5vq2jz6hjwzblnfvqr6rizbxllyst3wkcnfz7np2lskjsmrl2tqd.onion:8327");
        vSeeds.emplace_back("ig32f2l3xphumkptinc5j5ic4kj6mdyhobcfctcbkpfg4rhiuvbojmid.onion:8327");
        vSeeds.emplace_back("74zt3nxpo4sjjix47crkasbjk3gbsngrcqe4wi3q55aytjhns3ajyqqd.onion:8327");

        vFixedSeeds = std::vector<SeedSpec6>(
            pnSeed6_main,
            pnSeed6_main + ARRAYLEN(pnSeed6_main)
        );

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 28);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1, 210);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x0E, 0xC9, 0x61};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x0E, 0xC9, 0x4C};
        bech32_hrp = "rxb";

        fDefaultConsistencyChecks = false;
        fRequireStandard          = true;
        m_is_test_chain           = false;
        m_is_mockable_chain       = false;

        checkpointData = {
            {
                {0, consensus.hashGenesisBlock},
                {700, uint256S("0x00000000649f7028b0a07cf22235a72ec2b1f576f2e6ffb51bd7adc6be16cbf8")}
            }
        };

        chainTxData = ChainTxData{
            1773464932,
            700,
            0.0006
        };
    }
};

static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams& Params()
{
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}
