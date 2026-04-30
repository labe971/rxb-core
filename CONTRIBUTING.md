# Contributing to Record XCore Blockchain (RXB)
============================


 RXB is an  cryptocurrency project 
 focused on providing a decentralized peer-to-peer digital currency system. 
 This document explains how you can contribute to the RXB network
 specifically by adding seed nodes.

## About Record XCore Blockchain (RXB)


RXB-core  is a hard fork of Bitcoin with the following key characteristics:

- **Block time**: 10 minutes
- **Block reward**: 100 RXB (starting)
- **Halving interval**: 195,000 blocks
- **Maximum block size**: 6 MB
- **Network ports**: 8327 (P2P), 8328 (RPC)
- **Magic bytes**: 0xD9 0x51 0xAE 0x87
- **No testnet or regtest support** - mainnet only

## How to Contribute


 Currently the primary way to contribute to RXB is by running a seed node 
 to help bootstrap the network.

### Adding New Seed Nodes


To add a new seed node to the RXB network:

#### Requirements for Your Node:

- Must run RXB 24/7 with good uptime
- Must have port 8327 publicly accessible
- Minimum 10 MBit/s network connection recommended
- Must follow RXB consensus rules
- Must not be behind restrictive firewalls or NAT

#### How to Submit a Seed:

 Fork the RXB repository: `https://github.com/Heiwabit/rxb.git`
 Edit the file `contrib/seeds/seeds.txt`
 Add your seed in the format: `address:port` (one per line)
 Submit a Pull Request

#### Accepted Address Formats:

 IPv4 addresses

 192.168.1.1:8327
 IPv6 addresses (with brackets)
 [2001:db8::1]: 8327
 Tor onion addresses (recommended for privacy)

 youraddress.onion:8327


#### Privacy Note:

- All seeds are treated equally in the codebase
- No ownership information is stored with seeds
- Regular rotation of seed order ensures anonymity
- Consider using Tor hidden services for enhanced privacy

## Current Development Status

  Important  
  
  RXB is currently in /development phase. 
  
  The code is provided "as-it is!" without warranty of any kind.

### What Works:

- Basic RXB
- Custom genesis block with RXB parameters
- Network separation via custom ports and magic bytes
- OP_RETURN support up to 300 bytes

### What's Different from Bitcoin:

- Different address prefixes (B instead of 1/3)
- Custom Bech32 HRP ("rxb" instead of "bc")
- No testnet or regtest support
- Different subsidy halving schedule
- Modified difficulty parameters

## Building from Source

To build RXB from source:


```bash

# Clone the repository

git clone https://github.com/Heiwabitnull/rxb-core.git
cd rxb

# Install dependencies (Ubuntu/Debian)

sudo apt-get install build-essential libtool autotools-dev automake pkg-config bsdmainutils python3 libevent-dev libboost-dev libsqlite3-dev

# Build

      ./autogen.sh

      ./configure

      make -j$(nproc)

# Running a Node

# Start RXB daemon

  ./src/rxbd -daemon

# Check node status

  ./src/rxb-cli getblockchaininfo

# View network connections

  ./src/rxb-cli getpeerinfo
   
#and so on 

Community Channels

 Currently RXB development discussion happens through GitHub issues and
 pull requests. As the project grows additional communication channels 
 may be established.

 You can also support RXB by creating discussion channels and 
 supporting the creation of services and websites 
 to which you can add additional seednodes.

# Code of Conduct

    Please be respectful and constructive in all communications

    Focus on technical merits of contributions

    Help maintain a welcoming environment for new contributors

    Report any issues or concerns through GitHub issues

# License

   By contributing to RXB
   you agree that your contributions will be licensed under the MIT license 
   unless specified otherwise at the top of the file. 
   All work must include appropriate license headers.


# Experimental Nature Disclaimer

   Warning!
 
   RXB is  software.
 
   Use at your own risk.
 
   The developers make no warranties regarding the security

   stability or functionality of the network.
   
   The creators of RXB take no responsibility for anything
 
   that may happen while using this software.

   This includes zombie apocalypses

   your ex running on unstable emotional os

   as well as whoever you decide to vote for.
 
   Use at your own risk keep your system updated and stay calm mate
 

   ### Project Independence Statement

  RXB is an independent cryptocurrency project
  that draws inspiration from early decentralized protocols and
  their underlying principles of decentralization,
  transparency and cryptographic security.

  RXB acknowledges the foundational work of early cryptocurrency pioneers and
  the open source community in developing decentralized digital currency systems.

  RXB focuses on simplicity effectiveness and accessibility for everyone.
  It is built on proven foundations while keeping the system minimal and practical.
  The project maintains core values of open-source development and decentralized consensus.

  ### Network Support

  Anyone with a VPS is encouraged to support the network by running bridge nodes
  to help prevent network splits and improve connectivity.

  ### Record XCore Blockchain (RXB)  version: 0.3.0
