// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Copyright (c) 2025 The RXB developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <miner.h>

#include <amount.h>
#include <chain.h>
#include <chainparams.h>
#include <coins.h>
#include <consensus/consensus.h>
#include <consensus/merkle.h>
#include <consensus/tx_verify.h>
#include <consensus/validation.h>
#include <policy/feerate.h>
#include <policy/policy.h>
#include <pow.h>
#include <primitives/transaction.h>
#include <timedata.h>
#include <util/moneystr.h>
#include <util/system.h>
#include <validation.h>
#include <net_processing.h>
#include <wallet/wallet.h>
#include <key_io.h>
#include <rpc/server.h>
#include <rpc/util.h>
#include <shutdown.h>
#include <rpc/blockchain.h>
#include <util/time.h>

#include <algorithm>
#include <utility>
#include <chrono>
#include <thread>

// RXB: Declare global connection manager from init.cpp
extern CConnman* g_connman;

// Global miner instance - MUST be defined in exactly one .cpp file
std::unique_ptr<RXBMiner> g_miner;

// Static members of BlockAssembler must be defined
boost::optional<int64_t> BlockAssembler::m_last_block_num_txs;
boost::optional<int64_t> BlockAssembler::m_last_block_weight;

// Mining statistics
static std::atomic<int64_t> nLastHashCount(0);
static std::atomic<int64_t> nLastHashTime(0);

BlockAssembler::Options::Options() {
    blockMinFeeRate = CFeeRate(RXB_DEFAULT_BLOCK_MIN_TX_FEE);
    nBlockMaxWeight = DEFAULT_BLOCK_MAX_WEIGHT;
}

BlockAssembler::BlockAssembler(const CTxMemPool& mempool, const CChainParams& params, const Options& options)
    : chainparams(params),
      m_mempool(mempool)
{
    blockMinFeeRate = options.blockMinFeeRate;
    nBlockMaxWeight = options.nBlockMaxWeight;
    
    // Initialize other members
    pblock = nullptr;
    fIncludeWitness = false;
    nBlockWeight = 0;
    nBlockTx = 0;
    nBlockSigOpsCost = 0;
    nFees = 0;
    nHeight = 0;
    nLockTimeCutoff = 0;
}

// Helper function to get default options
static BlockAssembler::Options DefaultOptions()
{
    BlockAssembler::Options options;
    options.nBlockMaxWeight = gArgs.GetArg("-blockmaxweight", DEFAULT_BLOCK_MAX_WEIGHT);
    CAmount n = 0;
    if (gArgs.IsArgSet("-blockmintxfee") && ParseMoney(gArgs.GetArg("-blockmintxfee", ""), n)) {
        options.blockMinFeeRate = CFeeRate(n);
    } else {
        options.blockMinFeeRate = CFeeRate(RXB_DEFAULT_BLOCK_MIN_TX_FEE);
    }
    return options;
}

BlockAssembler::BlockAssembler(const CTxMemPool& mempool, const CChainParams& params)
    : BlockAssembler(mempool, params, DefaultOptions()) {}

void BlockAssembler::resetBlock()
{
    inBlock.clear();

    nBlockWeight = 4000;
    nBlockSigOpsCost = 400;
    fIncludeWitness = false;

    nBlockTx = 0;
    nFees = 0;
    
    pblocktemplate.reset();
    pblock = nullptr;
}

std::unique_ptr<CBlockTemplate> BlockAssembler::CreateNewBlock(const CScript& scriptPubKeyIn)
{
    int64_t nTimeStart = GetTimeMicros();

    resetBlock();

    pblocktemplate.reset(new CBlockTemplate());

    if(!pblocktemplate.get())
        return nullptr;
    pblock = &pblocktemplate->block;

    pblock->vtx.emplace_back();
    pblocktemplate->vTxFees.push_back(-1);
    pblocktemplate->vTxSigOpsCost.push_back(-1);

    LOCK2(cs_main, m_mempool.cs);
    CBlockIndex* pindexPrev = ::ChainActive().Tip();
    assert(pindexPrev != nullptr);
    nHeight = pindexPrev->nHeight + 1;

    pblock->nVersion = ComputeBlockVersion(pindexPrev, chainparams.GetConsensus());
    if (chainparams.MineBlocksOnDemand())
        pblock->nVersion = gArgs.GetArg("-blockversion", pblock->nVersion);

    pblock->nTime = GetAdjustedTime();
    const int64_t nMedianTimePast = pindexPrev->GetMedianTimePast();

    nLockTimeCutoff = (STANDARD_LOCKTIME_VERIFY_FLAGS & LOCKTIME_MEDIAN_TIME_PAST)
                       ? nMedianTimePast
                       : pblock->GetBlockTime();

    fIncludeWitness = IsWitnessEnabled(pindexPrev, chainparams.GetConsensus());

    int nPackagesSelected = 0;
    int nDescendantsUpdated = 0;
    addPackageTxs(nPackagesSelected, nDescendantsUpdated);

    int64_t nTime1 = GetTimeMicros();

    m_last_block_num_txs = nBlockTx;
    m_last_block_weight = nBlockWeight;

    CMutableTransaction coinbaseTx;
    coinbaseTx.vin.resize(1);
    coinbaseTx.vin[0].prevout.SetNull();
    coinbaseTx.vout.resize(1);
    coinbaseTx.vout[0].scriptPubKey = scriptPubKeyIn;
    coinbaseTx.vout[0].nValue = nFees + GetBlockSubsidy(nHeight, chainparams.GetConsensus());
    coinbaseTx.vin[0].scriptSig = CScript() << nHeight << OP_0;
    pblock->vtx[0] = MakeTransactionRef(std::move(coinbaseTx));
    pblocktemplate->vchCoinbaseCommitment = GenerateCoinbaseCommitment(*pblock, pindexPrev, chainparams.GetConsensus());
    pblocktemplate->vTxFees[0] = -nFees;

    LogPrintf("CreateNewBlock(): block weight: %u txs: %u fees: %ld sigops %d\n", GetBlockWeight(*pblock), nBlockTx, nFees, nBlockSigOpsCost);

    pblock->hashPrevBlock  = pindexPrev->GetBlockHash();
    UpdateTime(pblock, chainparams.GetConsensus(), pindexPrev);
    pblock->nBits          = GetNextWorkRequired(pindexPrev, pblock, chainparams.GetConsensus());
    pblock->nNonce         = 0;
    pblocktemplate->vTxSigOpsCost[0] = WITNESS_SCALE_FACTOR * GetLegacySigOpCount(*pblock->vtx[0]);

    BlockValidationState state;
    if (!TestBlockValidity(state, chainparams, *pblock, pindexPrev, false, false)) {
        throw std::runtime_error(strprintf("%s: TestBlockValidity failed: %s", __func__, state.ToString()));
    }
    int64_t nTime2 = GetTimeMicros();

    LogPrint(BCLog::BENCH, "CreateNewBlock() packages: %.2fms (%d packages, %d updated descendants), validity: %.2fms (total %.2fms)\n", 0.001 * (nTime1 - nTimeStart), nPackagesSelected, nDescendantsUpdated, 0.001 * (nTime2 - nTime1), 0.001 * (nTime2 - nTimeStart));

    return std::move(pblocktemplate);
}

void BlockAssembler::onlyUnconfirmed(CTxMemPool::setEntries& testSet)
{
    for (CTxMemPool::setEntries::iterator iit = testSet.begin(); iit != testSet.end(); ) {
        if (inBlock.count(*iit)) {
            testSet.erase(iit++);
        }
        else {
            iit++;
        }
    }
}

bool BlockAssembler::TestPackage(uint64_t packageSize, int64_t packageSigOpsCost) const
{
    if (nBlockWeight + WITNESS_SCALE_FACTOR * packageSize >= nBlockMaxWeight)
        return false;
    if (nBlockSigOpsCost + packageSigOpsCost >= MAX_BLOCK_SIGOPS_COST)
        return false;
    return true;
}

bool BlockAssembler::TestPackageTransactions(const CTxMemPool::setEntries& package)
{
    for (CTxMemPool::txiter it : package) {
        if (!IsFinalTx(it->GetTx(), nHeight, nLockTimeCutoff))
            return false;
        if (!fIncludeWitness && it->GetTx().HasWitness())
            return false;
    }
    return true;
}

void BlockAssembler::AddToBlock(CTxMemPool::txiter iter)
{
    pblock->vtx.emplace_back(iter->GetSharedTx());
    pblocktemplate->vTxFees.push_back(iter->GetFee());
    pblocktemplate->vTxSigOpsCost.push_back(iter->GetSigOpCost());
    nBlockWeight += iter->GetTxWeight();
    ++nBlockTx;
    nBlockSigOpsCost += iter->GetSigOpCost();
    nFees += iter->GetFee();
    inBlock.insert(iter);

    bool fPrintPriority = gArgs.GetBoolArg("-printpriority", DEFAULT_PRINTPRIORITY);
    if (fPrintPriority) {
        LogPrintf("fee %s txid %s\n",
                  CFeeRate(iter->GetModifiedFee(), iter->GetTxSize()).ToString(),
                  iter->GetTx().GetHash().ToString());
    }
}

int BlockAssembler::UpdatePackagesForAdded(const CTxMemPool::setEntries& alreadyAdded,
        indexed_modified_transaction_set &mapModifiedTx)
{
    int nDescendantsUpdated = 0;
    for (CTxMemPool::txiter it : alreadyAdded) {
        CTxMemPool::setEntries descendants;
        m_mempool.CalculateDescendants(it, descendants);
        for (CTxMemPool::txiter desc : descendants) {
            if (alreadyAdded.count(desc))
                continue;
            ++nDescendantsUpdated;
            modtxiter mit = mapModifiedTx.find(desc);
            if (mit == mapModifiedTx.end()) {
                CTxMemPoolModifiedEntry modEntry(desc);
                modEntry.nSizeWithAncestors -= it->GetTxSize();
                modEntry.nModFeesWithAncestors -= it->GetModifiedFee();
                modEntry.nSigOpCostWithAncestors -= it->GetSigOpCost();
                mapModifiedTx.insert(modEntry);
            } else {
                mapModifiedTx.modify(mit, update_for_parent_inclusion(it));
            }
        }
    }
    return nDescendantsUpdated;
}

bool BlockAssembler::SkipMapTxEntry(CTxMemPool::txiter it, indexed_modified_transaction_set &mapModifiedTx, CTxMemPool::setEntries &failedTx)
{
    assert(it != m_mempool.mapTx.end());
    return mapModifiedTx.count(it) || inBlock.count(it) || failedTx.count(it);
}

void BlockAssembler::SortForBlock(const CTxMemPool::setEntries& package, std::vector<CTxMemPool::txiter>& sortedEntries)
{
    sortedEntries.clear();
    sortedEntries.insert(sortedEntries.begin(), package.begin(), package.end());
    std::sort(sortedEntries.begin(), sortedEntries.end(), CompareTxIterByAncestorCount());
}

void BlockAssembler::addPackageTxs(int &nPackagesSelected, int &nDescendantsUpdated)
{
    indexed_modified_transaction_set mapModifiedTx;
    CTxMemPool::setEntries failedTx;

    UpdatePackagesForAdded(inBlock, mapModifiedTx);

    CTxMemPool::indexed_transaction_set::index<ancestor_score>::type::iterator mi = m_mempool.mapTx.get<ancestor_score>().begin();
    CTxMemPool::txiter iter;

    const int64_t MAX_CONSECUTIVE_FAILURES = 1000;
    int64_t nConsecutiveFailed = 0;

    while (mi != m_mempool.mapTx.get<ancestor_score>().end() || !mapModifiedTx.empty()) {
        if (mi != m_mempool.mapTx.get<ancestor_score>().end() &&
            SkipMapTxEntry(m_mempool.mapTx.project<0>(mi), mapModifiedTx, failedTx)) {
            ++mi;
            continue;
        }

        bool fUsingModified = false;

        modtxscoreiter modit = mapModifiedTx.get<ancestor_score>().begin();
        if (mi == m_mempool.mapTx.get<ancestor_score>().end()) {
            iter = modit->iter;
            fUsingModified = true;
        } else {
            iter = m_mempool.mapTx.project<0>(mi);
            if (modit != mapModifiedTx.get<ancestor_score>().end() &&
                    CompareTxMemPoolEntryByAncestorFee()(*modit, CTxMemPoolModifiedEntry(iter))) {
                iter = modit->iter;
                fUsingModified = true;
            } else {
                ++mi;
            }
        }

        assert(!inBlock.count(iter));

        uint64_t packageSize = iter->GetSizeWithAncestors();
        CAmount packageFees = iter->GetModFeesWithAncestors();
        int64_t packageSigOpsCost = iter->GetSigOpCostWithAncestors();
        if (fUsingModified) {
            packageSize = modit->nSizeWithAncestors;
            packageFees = modit->nModFeesWithAncestors;
            packageSigOpsCost = modit->nSigOpCostWithAncestors;
        }

        if (packageFees < blockMinFeeRate.GetFee(packageSize)) {
            return;
        }

        if (!TestPackage(packageSize, packageSigOpsCost)) {
            if (fUsingModified) {
                mapModifiedTx.get<ancestor_score>().erase(modit);
                failedTx.insert(iter);
            }

            ++nConsecutiveFailed;

            if (nConsecutiveFailed > MAX_CONSECUTIVE_FAILURES && nBlockWeight >
                    nBlockMaxWeight - 4000) {
                break;
            }
            continue;
        }

        CTxMemPool::setEntries ancestors;
        uint64_t nNoLimit = std::numeric_limits<uint64_t>::max();
        std::string dummy;
        m_mempool.CalculateMemPoolAncestors(*iter, ancestors, nNoLimit, nNoLimit, nNoLimit, nNoLimit, dummy, false);

        onlyUnconfirmed(ancestors);
        ancestors.insert(iter);

        if (!TestPackageTransactions(ancestors)) {
            if (fUsingModified) {
                mapModifiedTx.get<ancestor_score>().erase(modit);
                failedTx.insert(iter);
            }
            continue;
        }

        nConsecutiveFailed = 0;

        std::vector<CTxMemPool::txiter> sortedEntries;
        SortForBlock(ancestors, sortedEntries);

        for (size_t i=0; i<sortedEntries.size(); ++i) {
            AddToBlock(sortedEntries[i]);
            mapModifiedTx.erase(sortedEntries[i]);
        }

        ++nPackagesSelected;
        nDescendantsUpdated += UpdatePackagesForAdded(ancestors, mapModifiedTx);
    }
}

// RXB Miner Implementation
RXBMiner::RXBMiner(CConnman* connman) :
    fMining(false),
    miningReward(0),
    connman(connman),
    hashCount(0),
    blockCount(0),
    hashRate(0.0),
    fStopMining(false) {
}

RXBMiner::~RXBMiner() {
    StopMining();
}

bool RXBMiner::StartMining(const std::string& address, int nThreads) {
    if (fMining.load()) {
        LogPrintf("Mining already started\n");
        return false;
    }
    
    CTxDestination dest = DecodeDestination(address);
    if (!IsValidDestination(dest)) {
        LogPrintf("Invalid RXB address: %s\n", address);
        return false;
    }
    
    miningAddress = address;
    fMining.store(true);
    fStopMining.store(false);
    
    hashCount.store(0);
    hashRate.store(0.0);
    
    for (int i = 0; i < nThreads && i < 10; ++i) {
        miningThreads.push_back(std::thread(&RXBMiner::MiningThread, this, address, i));
    }
    
    // Start block processor thread
    std::thread processorThread(&RXBMiner::ProcessMinedBlocks, this);
    processorThread.detach();
    
    LogPrintf("RXB mining started for address: %s with %d threads\n", address, nThreads);
    return true;
}

void RXBMiner::StopMining() {
    if (!fMining.load()) {
        return;
    }
    
    fStopMining.store(true);
    
    // Wait for mining thread to finish
    for (auto& thread : miningThreads) {
        if (thread.joinable()) thread.join();
    }
    miningThreads.clear();
    
    fMining.store(false);
    
    LogPrintf("RXB mining stopped\n");
}

void RXBMiner::GetMiningStats(uint64_t& hashes, uint64_t& blocks, double& rate) {
    hashes = hashCount.load();
    blocks = blockCount.load();
    rate = hashRate.load();
}

std::unique_ptr<CBlockTemplate> RXBMiner::CreateNewBlock(const CScript& scriptPubKey) {
    CTxMemPool& mempool = ::mempool;
    const CChainParams& chainparams = Params();
    
    BlockAssembler::Options options;
    options.nBlockMaxWeight = gArgs.GetArg("-blockmaxweight", DEFAULT_BLOCK_MAX_WEIGHT);
    options.blockMinFeeRate = CFeeRate(RXB_DEFAULT_BLOCK_MIN_TX_FEE);
    
    BlockAssembler assembler(mempool, chainparams, options);
    return assembler.CreateNewBlock(scriptPubKey);
}

bool RXBMiner::MineBlock(CBlock& block, const CChainParams& chainparams) {
    uint32_t nBits = block.nBits;
    
    arith_uint256 hashTarget;
    bool fNegative, fOverflow;
    hashTarget.SetCompact(nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow)
        return false;
    
    uint32_t nNonce = block.nNonce;
    int64_t nStartTime = GetTimeMillis();
    uint64_t localHashCount = 0;
    
    while (fMining.load() && !fStopMining.load() && !ShutdownRequested()) {
        localHashCount++;
        hashCount.fetch_add(1, std::memory_order_relaxed);
        
        if (GetAdjustedTime() > block.nTime + 60) {
            block.nTime = GetAdjustedTime();
        }
        
        block.nNonce = nNonce;
        uint256 hash = block.GetHash();
        
        if (UintToArith256(hash) <= hashTarget) {
            block.nNonce = nNonce;
            LogPrintf("RXB Miner: Block found! Hash: %s\n", hash.ToString());
            
            // Update hash rate
            int64_t currentTime = GetTimeMillis();
            int64_t elapsed = currentTime - nStartTime;
            if (elapsed > 0) {
                hashRate.store((localHashCount * 1000.0) / elapsed);
            }
            
            return true;
        }
        
        if (++nNonce == 0) {
            block.nTime = std::max(block.nTime + 1, (uint32_t)GetAdjustedTime());
        }
        
        // Update hash rate every 5 seconds
        if (GetTimeMillis() - nStartTime > 5000) {
            int64_t currentTime = GetTimeMillis();
            int64_t elapsed = currentTime - nStartTime;
            if (elapsed > 0) {
                hashRate.store((localHashCount * 1000.0) / elapsed);
                nStartTime = currentTime;
                localHashCount = 0;
            }
        }
        
        if (nNonce % 10000 == 0) {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    }
    
    return false;
}

bool RXBMiner::CheckProofOfWork(const uint256& hash, uint32_t nBits, const Consensus::Params& params) {
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    if (UintToArith256(hash) > bnTarget)
        return false;

    return true;
}

void RXBMiner::UpdateTime(CBlockHeader* pblock, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev) {
    int64_t nOldTime = pblock->nTime;
    int64_t nNewTime = std::max(pindexPrev->GetMedianTimePast() + 1, GetAdjustedTime());

    if (nOldTime < nNewTime)
        pblock->nTime = nNewTime;

}

void RXBMiner::MiningThread(const std::string& address, int threadId) {
    LogPrintf("RXB Mining Thread %d started\n", threadId);
    
    CTxDestination dest = DecodeDestination(address);
    CScript scriptPubKey = GetScriptForDestination(dest);
    
    while (fMining.load() && !fStopMining.load() && !ShutdownRequested()) {
        try {
            auto pblocktemplate = CreateNewBlock(scriptPubKey);
            if (!pblocktemplate) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            
            CBlock block = pblocktemplate->block;
            unsigned int nExtraNonce = 0;
            IncrementExtraNonce(&block, ChainActive().Tip(), nExtraNonce);
            
            if (MineBlock(block, Params())) {
                {
                    std::lock_guard<std::mutex> lock(cs_miner);
                    minedBlocksQueue.push(std::make_pair(block.GetHash(), block));
                }
                cv_miner.notify_one();
                
                blockCount.fetch_add(1, std::memory_order_relaxed);
            }
            
        } catch (const std::exception& e) {
            LogPrintf("Mining thread error: %s\n", e.what());
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    LogPrintf("RXB Mining Thread %d stopped\n", threadId);
}

void RXBMiner::ProcessMinedBlocks() {
    while ((fMining.load() || !minedBlocksQueue.empty()) && !ShutdownRequested()) {
        std::unique_lock<std::mutex> lock(cs_miner);
        cv_miner.wait_for(lock, std::chrono::seconds(1));
        
        while (!minedBlocksQueue.empty()) {
            auto frontPair = minedBlocksQueue.front();
            uint256 hash = frontPair.first;
            CBlock block = frontPair.second;
            minedBlocksQueue.pop();
            
            lock.unlock();
            
            bool fNewBlock = false;
            
            // Process the mined block
            if (!ProcessNewBlock(Params(), std::make_shared<const CBlock>(block), true, &fNewBlock)) {
                LogPrintf("Failed to process mined block %s\n", hash.ToString());
            } else {
                LogPrintf("Successfully processed mined block: %s\n", hash.ToString());
                
                if (connman) {
                    // Notify peers about the new block
                    connman->ForEachNode([&hash](CNode* pnode) {
                        LogPrint(BCLog::NET, "Notifying peer about mined block %s\n", hash.ToString());
                    });
                }
            }
            
            lock.lock();
        }
    }
}

int64_t UpdateTime(CBlockHeader* pblock, const Consensus::Params& consensusParams, const CBlockIndex* pindexPrev)
{
    int64_t nOldTime = pblock->nTime;
    int64_t nNewTime = std::max(pindexPrev->GetMedianTimePast()+1, GetAdjustedTime());

    if (nOldTime < nNewTime)
        pblock->nTime = nNewTime;


    return nNewTime - nOldTime;
}

void IncrementExtraNonce(CBlock* pblock, const CBlockIndex* pindexPrev, unsigned int& nExtraNonce)
{
    static uint256 hashPrevBlock;
    if (hashPrevBlock != pblock->hashPrevBlock)
    {
        nExtraNonce = 0;
        hashPrevBlock = pblock->hashPrevBlock;
    }
    ++nExtraNonce;
    unsigned int nHeight = pindexPrev->nHeight+1;
    CMutableTransaction txCoinbase(*pblock->vtx[0]);
    txCoinbase.vin[0].scriptSig = (CScript() << nHeight << CScriptNum(nExtraNonce));
    assert(txCoinbase.vin[0].scriptSig.size() <= 100);

    pblock->vtx[0] = MakeTransactionRef(std::move(txCoinbase));
    pblock->hashMerkleRoot = BlockMerkleRoot(*pblock);
}

// RPC Implementation
UniValue startmining(const JSONRPCRequest& request) {
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
        throw std::runtime_error(
            "startmining \"address\" ( threads )\n"
            "\nStart RXB mining.\n"
            "\nArguments:\n"
            "1. \"address\"    (string, required) RXB address to receive mined coins\n"
            "2. threads       (numeric, optional, default=1) Number of mining threads\n"
            "\nExamples:\n"
            + HelpExampleCli("startmining", "\"RXBaddress...\"")
            + HelpExampleCli("startmining", "\"RXBaddress...\" 4")
            + HelpExampleRpc("startmining", "\"RXBaddress...\", 4")
        );
    
    std::string address = request.params[0].get_str();
    int threads = 1;
    if (request.params.size() > 1) {
        if (request.params[1].isNum()) threads = request.params[1].get_int();
        else threads = atoi(request.params[1].get_str().c_str());
    }
    
    if (threads < 1 || threads > 10)
        throw JSONRPCError(RPC_INVALID_PARAMETER, "Invalid number of threads");
    
    if (!g_miner) {
        g_miner = std::unique_ptr<RXBMiner>(new RXBMiner(g_connman));
    }
    
    bool success = g_miner->StartMining(address, threads);
    
    UniValue result(UniValue::VOBJ);
    result.pushKV("started", success);
    result.pushKV("address", address);
    result.pushKV("threads", threads);
    
    return result;
}

UniValue stopmining(const JSONRPCRequest& request) {
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "stopmining\n"
            "\nStop RXB mining.\n"
            "\nExamples:\n"
            + HelpExampleCli("stopmining", "")
            + HelpExampleRpc("stopmining", "")
        );
    
    if (!g_miner) {
        throw JSONRPCError(RPC_IN_WARMUP, "Miner not initialized");
    }
    
    g_miner->StopMining();
    
    UniValue result(UniValue::VOBJ);
    result.pushKV("stopped", true);
    
    return result;
}

UniValue getmininginfo(const JSONRPCRequest& request) {
    if (request.fHelp || request.params.size() != 0)
        throw std::runtime_error(
            "getmininginfo\n"
            "\nReturns mining-related information.\n"
            "\nResult:\n"
            "{\n"
            "  \"blocks\": nnn,             (numeric) The current block\n"
            "  \"currentblockweight\": nnn, (numeric) The last block weight\n"
            "  \"currentblocktx\": nnn,     (numeric) The last block transaction\n"
            "  \"difficulty\": xxx.xxxxx    (numeric) The current difficulty\n"
            "  \"networkhashps\": xxx.xxxxx (numeric) The network hashes per second\n"
            "  \"pooledtx\": n              (numeric) The size of the mempool\n"
            "  \"chain\": \"xxxx\",           (string) current network name\n"
            "  \"warnings\": \"...\"          (string) any network warnings\n"
            "  \"mining\": true/false,      (boolean) If mining is active\n"
            "  \"hashrate\": xxx.xxxxx,     (numeric) Your hashrate in H/s\n"
            "  \"hashestotal\": n,          (numeric) Total hashes computed\n"
            "  \"blocksmined\": n           (numeric) Blocks mined by this node\n"
            "}\n"
            "\nExamples:\n"
            + HelpExampleCli("getmininginfo", "")
            + HelpExampleRpc("getmininginfo", "")
        );
    
    LOCK(cs_main);
    
    UniValue obj(UniValue::VOBJ);
    obj.pushKV("blocks",           (int)::ChainActive().Height());
    obj.pushKV("currentblockweight", (uint64_t)(BlockAssembler::m_last_block_weight.get_value_or(0)));
    obj.pushKV("currentblocktx",   (uint64_t)(BlockAssembler::m_last_block_num_txs.get_value_or(0)));
    double difficulty = GetDifficulty(::ChainActive().Tip());
    obj.pushKV("difficulty", difficulty);
    
    CTxMemPool& mempool = ::mempool;
    obj.pushKV("networkhashps",    0); // Not implemented for now
    obj.pushKV("pooledtx",         (uint64_t)mempool.size());
    obj.pushKV("chain",            Params().NetworkIDString());
    
    // Temporary solution for GetWarnings
    std::string warnings = "";
    // TODO: Add GetWarnings() implementation when available
    obj.pushKV("warnings", warnings);
    
    if (g_miner) {
        uint64_t hashes, blocks;
        double rate;
        g_miner->GetMiningStats(hashes, blocks, rate);
        
        obj.pushKV("mining", g_miner->IsMining());
        obj.pushKV("hashrate", rate);
        obj.pushKV("hashestotal", (int64_t)hashes);
        obj.pushKV("blocksmined", (int64_t)blocks);
    } else {
        obj.pushKV("mining", false);
        obj.pushKV("hashrate", 0.0);
        obj.pushKV("hashestotal", 0);
        obj.pushKV("blocksmined", 0);
    }
    
    return obj;
}

UniValue setgenerate(const JSONRPCRequest& request) {
    if (request.fHelp || request.params.size() < 1 || request.params.size() > 2)
        throw std::runtime_error(
            "setgenerate generate ( genproclimit )\n"
            "\nSet 'generate' true or false to turn generation on or off.\n"
            "Generation is limited to 'genproclimit' processors, -1 is unlimited.\n"
            "See the getgenerate call for the current setting.\n"
            "\nArguments:\n"
            "1. generate         (boolean, required) Set to true to turn on generation, false to turn off.\n"
            "2. genproclimit     (numeric, optional) Set the processor limit for when generation is on. Can be -1 for unlimited.\n"
            "\nExamples:\n"
            "\nSet the generation on with a limit of one processor\n"
            + HelpExampleCli("setgenerate", "true 1") +
            "\nCheck the setting\n"
            + HelpExampleCli("getgenerate", "") +
            "\nTurn off generation\n"
            + HelpExampleCli("setgenerate", "false") +
            "\nUsing json rpc\n"
            + HelpExampleRpc("setgenerate", "true, 1")
        );
    
    if (request.params[0].get_bool()) {
        throw JSONRPCError(RPC_INVALID_REQUEST, 
            "Use startmining with address parameter instead. Example: startmining \"your_RXB_address_here\"");
    } else {
        if (!g_miner) {
            throw JSONRPCError(RPC_IN_WARMUP, "Miner not initialized");
        }
        g_miner->StopMining();
    }
    
    return NullUniValue;
}


// Register RPC commands
