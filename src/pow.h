// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2025 RXB developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RXB_POW_H
#define RXB_POW_H

#include <consensus/params.h>

class CBlockHeader;
class CBlockIndex;
class uint256;

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params);
unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader* pblock, const Consensus::Params& params);

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params);

#endif // RXB_POW_H
