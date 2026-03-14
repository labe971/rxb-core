White Paper

Record XCore Blockchain (RXB)
Distributed Value Transfer and Evidential Archiving System


Abstract

RXB-core  is a system designed for distributed value transfer and immutable evidence archiving built as a hardfork 
based on proven distributed ledger solutions.

Project maintains fundamental principles of security and decentralisation 
while introducing optimisations in throughput and data archiving as well as network resilience.

RXB was designed to enable at least 1,7 million to possibly 2.8 million transactions per day 
making it suitable for everyday payments and daily value transfer.

RXB promotes universal accessibility by enabling full nodes to run on standard computer hardware 
and actively supports building infrastructure that increases privacy and resistance to censorship.

Ability to run RXB on older computers even 10 years old has real impact on ecology 
as it contributes to reduction of electronic waste.
 
Ability to install on older computers also prevents discrimination against people 
and enables everyone to join in technological progress.

Network starts as onion only network and every user is encouraged to create and maintain bridge nodes.
 
Users wishing to contribute to preventing netsplit can submit bridge node IP addresses to Heiwabit0@protonmail.com 
Users may also create forums to exchange bridge node IPs which can be placed in configuration files that they can promote.

RXB rejects implementation of protocols 
such as ordinals which unnecessarily increase blockchain size without added value for basic system function.

Project is fully open source and its goal is also daily use value transfer.

1. Introduction


Distributed ledger technology has opened new possibilities for value transfer and data storage in decentralised manner.

First implementations based on Proof of Work algorithm have proven their durability and security 
however with growing adoption challenges related to throughput and efficiency have emerged.


RXB was created with belief that it is possible to maintain fundamental advantages of original solutions 
while introducing targeted improvements.

Project focuses on practical usability and data purity as well as infrastructure accessibility
thus clearly distinguishing itself from practices that treat blockchain as space for storing arbitrary data 
and focusing on its fundamental role as value transfer system for daily use and notary for key information.

1.2 Goals and design principles


Maintaining Proof of Work consensus algorithm and peer to peer model as foundations.

Ensuring parameters that enable handling significant transaction volume 
without creating entry barriers for individual node operators.

Introducing standard and secure mechanism for irreversible recording of data existence evidence.

Supporting and promoting network topology connecting different communication layers to ensure continuity of operation.

Providing clearly defined open protocol whose operation is understandable and predictable.

2. Cryptographic foundations


Security of RXB is based on cryptography using elliptic curves specifically secp256k1 curve.

Mathematical properties of this curve enable generation of cryptographic key pairs namely private key and public key.

Private key is secret random number used to create digital signatures. 
Based on it through specific mathematical operation on elliptic curve corresponding public key is generated.

Process is unidirectional meaning that knowing public key one cannot calculate private key.

Public key can be freely shared and is used to verify authenticity of signatures created using associated private key.

2.2 Address creation


To facilitate system use public keys are not used directly as identifiers.

Instead they undergo transformation process into addresses.

Process of creating address consists of public key being processed through SHA-256 hash function.

Result of this processing is then processed through RIPEMD-160 function.

Hash obtained in this way is encoded into user readable text format.
For example starting with ebx1 for Segregated Witness type addresses.

Address is public identifier to which coins can be received and it does not directly contain public key 
which constitutes additional security layer.

3. Transaction signing and verification


When user wants to send coins they create transaction specifying recipient address and amount. 
Using their private key they generate digital signature specific to that transaction.

Signature is attached to transaction and network nodes to verify its validity 
use corresponding address and indirectly public key of sender as well as attached signature.

Verification consists of checking mathematical relationship.

If transaction was changed after signing verification will fail and transaction will be rejected. 
Private key remains secret and is never transmitted or revealed on network.

4. Security and wallet management


User private keys are stored in encrypted form in local file on their computer. 
Encryption by user of folder where private keys are stored is most important security element on user side.

4.1 Wallet protection

By default wallet is not encrypted so when creating wallet it is recommended to encrypt wallet folder 
by using strong wallet access password. 
This is possible in terminal using command lines but also in graphical wallet application.

Encrypting wallet file prevents access to funds in case of attempted unauthorised access to user files.

Users have complete freedom in creating and applying security methods. 
They can generate complex passwords using tools such as openssl on Linux systems 
or use their own well thought out phrases.

Responsibility for security of passwords and private keys rests solely with user.

4.2 Creating backups


Due to irreversible nature of value storage on network regular creation of wallet file backups is essential.

Backup should be stored in several physically separate and secure locations.

Before placing backup in any remote service it should be encrypted.

5. Network beginning is Genesis Block


RXB blockchain begins with Genesis Block which is block number zero.

This is founding block often called creation block which initiates network and establishes its initial parameters. 
It contains symbolic reference to idea of decentralised system.

Key feature of RXB Genesis Block is structure of its first transaction which is generating transaction.

Reward for this block is not assigned to address controlled by creator. 
Instead output of this transaction is deliberately and irreversibly directed to OP_RETURN mechanism.

In practice this means that coins generated in Genesis Block cannot be spent by anyone.

6. Proof of Work and consensus security


Consensus in RXB is based on Proof of Work mechanism 
which is algorithm where miners perform cryptographic calculations trying to find SHA-256 hash value of block header 
that meets specific difficulty condition being less than established target value.

Operation is deliberately computationally intensive. 
Finding correct solution requires performing huge number of attempts.

Finding block is random process and probability of finding correct solution in given unit of time. 
It is proportional to computational power provided.

Node that first finds solution has right to announce new block and receive reward for it.

Reward consists of two parts namely newly issued coins as block reward and sum of transaction fees contained in block.

6.1 Emission parameters and halving


Emission of new coins occurs according to following parameters.

Halving interval every 195000 blocks

Block generation time 10 minutes as target

Coinbase maturity 50 blocks meaning coinbase transactions can be spent only after 50 subsequent blocks

Halving mechanism causes reward for mining block to decrease by half every 195000 blocks. 
This deflationary emission model ensures that coins are resource with predictable supply.

6.2 Difficulty adjustment and target block time


RXB uses LWMA difficulty adjustment algorithm 
which recalculates mining difficulty after every single block based on timestamps and difficulties of recent blocks.

Target time per block is 10 minutes which is 600 seconds.

LWMA algorithm responds dynamically to changes in network hashrate 
by analyzing weighted moving average of recent block solving times.

If blocks are mined faster than target time difficulty increases forcing miners to perform greater number of calculations.

If mining is slower difficulty decreases.

This per block adjustment ensures stable and predictable coin emission schedule 
and regularity of block creation regardless of changes in total computational power of network.

6.3 Block structure and parameters


Block in RXB consists of two main parts being block header and list of transactions.

Maximum block weight is 6 million weight units allowing approximately 1.7 to 2.8 million transactions per day.

Block header is relatively small set of data containing protocol version 
and hash of previous block cryptographically linking this block to preceding block in chain 
as well as Merkle tree root for transactions and timestamp and target difficulty value 
and nonce value which is variable parameter used to find correct Proof of Work solution.

Transactions in block are organised into tree structure called Merkle tree.

For this purpose SHA-256 hash is calculated for each transaction.

These hashes are combined in pairs and new hash is calculated from each pair.

Pairing and hashing process repeats until single value is obtained namely Merkle tree root.

Root is then placed in block header.

Thanks to this structure one can efficiently and securely prove membership of single transaction in block 
by presenting only small set of hashes called Merkle path and not entire list of transactions.

Any change in any transaction causes change in root which is immediately detected by nodes.

6.4 Identifiers for block hash and transaction hash


Block hash being block identifier is SHA-256 hash calculated for entire block header 
and serves as its identifier in network.

Transaction hash being transaction identifier is SHA-256 hash calculated for entire transaction 
and serves as its identifier.

7. Evidential archiving using OP_RETURN


RXB extends standard use of OP_RETURN field in transactions.

It allows attachment of up to 300 bytes of data to transaction.

Data is then permanently and irreversibly recorded in blockchain.

Key feature is that data in OP_RETURN is specially marked as unspendable.

This means they do not create transaction output known as UTXO that could be spent later.

They therefore do not burden main state of ownership database remaining as historical digital record.

Main application is timestamping and proof of existence.

User can calculate cryptographic hash for example using SHA-256 of any document or code or contract or data set 
and then place this hash in OP_RETURN field of transaction.

What is recorded is therefore not document itself but its indisputable digital fingerprint.

By recording only hash mathematical proof is created that document existed at time transaction was included 
and therefore before specific block. 
Compliance with principle of data minimisation is maintained which is important in context of regulations such as GDPR.

Hash itself does not allow reconstruction of original personal data. 
Blockchain hygiene is maintained by avoiding its unnecessary growth.

Users can use their own encryption methods 
or additional hashing before placing data in OP_RETURN depending on their needs.

Responsibility for nature of recorded data and compliance with law rests with user.

8. Network and topology and privacy


RXB-core operates in peer to peer model.

It consists of many independent nodes communicating with each other directly.

Each full node maintains multiple connections with other nodes and also receives and verifies 
and forwards new transactions and blocks and stores complete blockchain independently verifying all consensus rules.

Structure eliminates central points increasing resistance to failures and censorship.

Network starts as onion only network providing maximum privacy and censorship resistance from beginning.

8.1 Node types and resilience building


Onion Node operates entirely as hidden service in Tor network 
and actual IP address is invisible to other participants of RXB.

Bridge Node is full node configured to connect both with nodes in public internet network known as clearnet 
and with onion nodes in Tor network.

It acts as bridge enabling users connecting through Tor to access RXB.

Every user is strongly encouraged to create and maintain bridge nodes. 
Running bridge nodes and onion nodes by users is crucial for building hybrid mesh type topology.

In such network communication paths are multiply redundant. 
Even if access to some nodes in clearnet is blocked network can still function through onion connections and bridges.

In initial phases of network bridge nodes play important role in ensuring connectivity 
for users wishing to maintain privacy as well as those connecting from clearnet.

Users wishing to contribute to preventing netsplit 
and strengthening network can submit their bridge node IP addresses to Heiwabit0@protonmail.com.

Users may also create their own forums to exchange bridge node IP addresses 
which can be placed among other things in configuration files that they can promote and share with community.

It is recommended to create bridge nodes by appropriately modifying configuration file 
in such way that node user can also at any time switch to use only onion 
when they deem that they need higher level of privacy.

8.2 Practical dimension of anonymity


Basic anonymity in RXB has pseudonymous character.

Transactions publicly link addresses not legal identities. 
However advanced analysis of flows between addresses known as chain analysis may 
allow conclusions to be drawn about connections.

Real anonymity in terms of hiding network activity is achieved only by separating IP address 
from activities on RXB network and this can be done in two main ways.

These methods are running or connecting through Onion Node where all communication is tunneled through Tor network 
or by connecting to network through Bridge Node using Tor client.

Without applying such measures IP address from which node announces transactions or connects to other nodes 
can be observed and potentially linked to specific addresses.

Anonymity is therefore relative and largely depends on actions and choices of user themselves.

It is essential that users use password in their node configuration file that is known only to them.

9. Segregated Witness functionality and transaction fees


RXB implements protocol called Segregated Witness from first block.

Main benefits are that this protocol solves theoretical weakness related to transaction identifier malleability.

Increased throughput is achieved because digital signatures being proofs are 
moved outside main part of transaction data and this allows more transactions to be placed in block 
while maintaining same size limit effectively increasing throughput.

Addresses using SegWit format on RXB are easily recognisable for example by rxb prefix.

9.1 Transaction fees


RXB is maintained by distributed node operators and miners.

To encourage inclusion of transaction in block sender can attach fee to it.

Fee amount is result of free market meaning it depends on current demand for space in block and on sender decision.

Wallet software has automatic mode in which wallet itself estimates current network fees and 
suggests amount that should ensure transaction confirmation in reasonable time.

User can also use manual mode in which they can independently and consciously set any fee amount.

Higher fee means faster transaction processing. 
User can also set fee to zero but then transaction may wait for confirmation for very long time 
or may never receive confirmation.

10. Built in mining tools


RXB node software contains integrated module enabling participation in mining process being Proof of Work.

Thanks to this every user running full node has ability to use computational power of their computer 
to compete for finding new block and receiving reward while simultaneously contributing to securing network.

This democratises block creation process.

RXB is fully open source project and its development 
is based on community of users and developers and researchers.

Project goal is to provide system for daily use value transfer that is accessible to everyone.

11. Summary


RXB is hardfork whose goal is practical and sustainable evolution of idea of decentralised ledger 
based on Proof of Work.

It combines proven conservative foundations with targeted improvements in areas of scalability 
and archiving and network resilience.

It is primarily technological tool and communication protocol and system of rules designed for daily use value transfer.

It is not controlled by any single person or company or government. 

It has no central server or management or customer service department.

Its existence and operation depend solely on voluntary participation of users around world who run software 

and jointly maintain network.

RXB is not financial instrument in traditional sense.

By running full RXB node user not only gains full independent system verification 

but also actively contributes to strengthening its decentralisation and resilience and freedom 

in terms of among other things daily value transfer.

12. Legal disclaimers


This document has informational and technical character and does not constitute legal or financial or investment advice.

Final binding protocol parameters are contained solely in publicly available source code of software.

Before taking any action it is recommended to familiarise yourself with RXB code.

User bears full responsibility for their decisions and compliance of their actions 

with law applicable in their jurisdiction.

Project website rxb.org

Source code repository https://github.com/Heiwabitnull/rxb.git

Contact for bridge node IP submissions Heiwabit0@protonmail.com

Creator Heiwabit.
 
Gratitude is owed to pioneers whose foundational contributions enabled practical distributed ledger systems.
 
Implementation incorporates LWMA difficulty adjustment algorithm developed by Zaworski
 
ensuring responsive network adaptation

RXB 
Allows users to record permanent Proof of Existence data via the OP_RETURN field. 
Simply each entry uses a separate address in the UTXO model every record remains autonomous.



 

I wish you success in mining. 

Be good and do good.

