

# RXB Core v0.3.0 Foxbat

Official website: https://rxbcore.org
Source code: https://github.com/Heiwabitnull/rxb-core



## IMPORTANT Quick Start for New Nodes

New nodes must use the bootstrap file for first-time synchronization.
Direct sync from genesis block is not supported in v0.3.0 Foxbat.

Step 1 Download bootstrap:
https://github.com/Heiwabitnull/rxb-core/releases/tag/v0.3.0-foxbat

Step 2 Extract before starting your node:
tar -xzf rxb-bootstrap-4852.tar.gz -C ~/

Step 3 Compile and start node normally.

## Overview


RXB is an open-source SHA-256 proof-of-work blockchain focused on
document notarization and decentralized record keeping.
No premine. No ICO. No investors. Just honest mining.

Block size is set to 6 megabytes.
OP_RETURN supports up to 300 bytes for data messages.
Network operates exclusively over Tor for privacy.
P2P port: 8327


## Key Features

  SHA-256 Proof of Work
  Bitcoin style difficulty retarget every 2016 blocks (v0.3.0 Foxbat)
  Asymmetric clamp: max +2% difficulty increase. Max -55% decrease per window
  SegWit from block 0
  Tor only network
  Block reward: 100 RXB
  Halving every 195,000 blocks
  Total supply: 39,000,000 RXB
  bech32 prefix is :  rxb
  Legacy address prefix: B

## Mining Pool


Public solo pool available:
stratum+tcp://135.181.255.219:3333
Worker: YOUR_OWN_RXB_LEGACY_ADDRESS
Password: x

Legacy addresses start with B.
Each block reward goes entirely to the miner who finds it.


## Compile for Linux


Install dependencies:

sudo apt install build-essential libtool autotools-dev automake pkg-config \
bsdmainutils python3 libssl-dev libevent-dev libboost-all-dev libdb++-dev tor

Clone repository:
git clone https://github.com/Heiwabitnull/rxb-core.git
cd rxb-core


Generate and configure:
./autogen.sh
./configure --disable-tests --disable-bench --without-miniupnpc \
            --with-gui=qt5 --enable-wallet --with-incompatible-bdb

Build.

make -j$(nproc)

Start Tor before running node:
sudo systemctl start tor

Run node.

./src/rxbd -daemon
sleep 30
./src/rxb-cli getblockcount

## Basic Commands

Create wallet.

./src/rxb-cli createwallet "mywallet"

Get legacy address for mining:

./src/rxb-cli getnewaddress "mining" legacy

Start CPU mining:
./src/rxb-cli startmining "YOUR_LEGACY_ADDRESS" 2

Check status:

./src/rxb-cli getmininginfo

## Seed Nodes

5v2v5vq2jz6hjwzblnfvqr6rizbxllyst3wkcnfz7np2lskjsmrl2tqd.onion:8327
ig32f2l3xphumkptinc5j5ic4kj6mdyhobcfctcbkpfg4rhiuvbojmid.onion:8327
74zt3nxpo4sjjix47crkasbjk3gbsngrcqe4wi3q55aytjhns3ajyqqd.onion:8327
135.181.255.219:8327 (its bridge -- thanks to rafael for providing the vps bridge )


## License

RXB is released under MIT License.
Project is based on Bitcoin Core version 20.
https://opensource.org/licenses/MIT
