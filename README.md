# RXB Integration 

Official website https://rxbcore.org/

Source code https://github.com/Heiwabitnull/rxb-core.git

## Download Binaries

Pre-compiled binaries for Windows and Linux are available at
https://github.com/Heiwabitnull/rxb-core-binaries.git

The repository contains two folders. 
RXBWin for Windows builds and RXBLinux for Linux builds.

## Overview

RXB is an experimental digital currency system based 
on Bitcoin technology. 
Block size is set to 6 megabytes and 
OP_RETURN allows up to 300 bytes for data messages. 
This provides reasonable transaction capacity and sufficient functionality.

One of fundamental principles of RXB is rejection of 
Ordinals protocol. 
Thanks to this approach blockchain grows 
at a steady moderate pace keeping full node size practical for home users.

System is balanced so home users and small entities can use 
it effectively and reduces influence of large corporations on network.

RXB follows the principles that guided early decentralized 
systems. Creating an independent financial network that operates 
without restrictions from governments or institutions. 
It gives users worldwide equal opportunities 
to participate and help network grow.

## Key Features

System includes SegWit support. 
Home users can run full nodes without high costs 
for hard drives or new hardware.

Older computers and laptops can mine blocks reducing electronic 
waste and reusing existing hardware. 
Main focus on home users without unusual protocols like Ordinals.

Minimises corporate influence and supports equal access for 
all participants. 
My wish is that RXB never adopts Ordinals protocol.

## Project Goals

Project aims to provide a fair chance for new generations of 
users and support growth of independent financial systems. 
It allows higher transaction capacity and maintains a simple clear 
and practical system.

## Development and Testing

RXB does not use a traditional testnet or regression tests.
Users who want to run tests should dig up new genesis to use for testing. 
This has a positive impact on the creation of new POW consensus-based systems.

## License

RXB is released under MIT License. 
Project is based on Bitcoin version 20 built thanks to the work of Bitcoin 
developers and Satoshi Nakamoto.

More information about MIT License 
https://opensource.org/licenses/MIT

## Summary

RXB is a digital currency system that supports high 
transaction volumes. 
It allows home users to participate easily 
and enables mining on older hardware. 
System reduces node maintenance costs and promotes decentralisation fairness 
and practical accessibility for all users.
Feel free to run full nodes and mine RXB coins on your home hardware.

Compilation for windows:
Navigate to your source directory. In this example we're using the Downloads folder.
Generate configuration files

# Go to source directory

cd ~/Downloads/RXBcore

# Generate

./autogen.sh

# Then
 
CONFIG_SITE=$PWD/depends/x86_64-w64-mingw32/share/config.site ./configure --prefix=/ --host=x86_64-w64-mingw32 --disable-bench

# Build

make -j$(nproc)

# Create installer

make deploy



Linux Compilation:

# Go to source directory

cd ~/Downloads/RXBcore

# Generate files

./autogen.sh

# Configure for Linux

./configure --prefix=$PWD/depends/x86_64-pc-linux-gnu --disable-bench

# Build

make -j$(nproc)



