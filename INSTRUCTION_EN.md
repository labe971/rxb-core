===============================================================================
RXBNetwork v0.1.0 - Complete Guide for Windows
===============================================================================

TABLE OF CONTENTS:
1. Basic Installation
2. Windows Firewall Configuration
3. Tor Installation and Hidden Service Creation
4. Creating Configuration File
5. Creating Your Own Onion Seed Node
6. Passwords and Security
7. Creating Clearnet-Tor Bridges
8. Supporting the RXBNetwork
9. Troubleshooting

===============================================================================
1. BASIC INSTALLATION
===============================================================================

OPTION A: INSTALLER (recommended)
File: rxb-0.1.0-win64-setup-unsigned.exe (17 MB)

Steps:
1. Download the installer file
2. Right-click -> "Run as administrator"
3. Click "Next" -> "Install" -> "Finish"
4. Icon will appear on desktop

OPTION B: PORTABLE
File: rxb-0.1.0-win64-portable.zip (49 MB)

Steps:
1. Download the ZIP file
2. Right-click -> "Extract all"
3. Open the "release" folder
4. Run: rxb-qt.exe

FIRST RUN:
- Program will create a new wallet automatically
- Set a STRONG wallet password
- Save the password in a secure place (CANNOT BE RECOVERED!)
- Blockchain synchronization will start automatically (10-60 min)


===============================================================================
2. WINDOWS FIREWALL CONFIGURATION
===============================================================================

RXBNetwork uses port 8327 for P2P communication with other nodes.

STEP BY STEP CONFIGURATION:

Step 1: Open Windows Firewall
- Press Windows key
- Type: "Windows Defender Firewall"
- Click "Windows Defender Firewall with Advanced Security"

Step 2: Create Inbound Rule
- In left panel click: "Inbound Rules"
- In right panel click: "New Rule"
- Select: "Port" -> Next
- Select: "TCP"
- Select: "Specific local ports"
- Enter: 8327
- Click: Next
- Select: "Allow the connection"
- Click: Next
- Check all profiles (Domain, Private, Public)
- Click: Next
- Name: "RXBNetwork P2P Port 8327"
- Description: "Port for RXBNetwork node"
- Click: Finish

Step 3: Create Outbound Rule
- In left panel click: "Outbound Rules"
- Repeat steps from above
- Name: "RXBNetwork P2P Outbound 8327"

Step 4: Check Router (optional, for full node)
- Log in to router panel (usually 192.168.1.1 or 192.168.0.1)
- Find "Port Forwarding" section
- Add rule:
  - External port: 8327
  - Internal port: 8327
  - Protocol: TCP
  - Device IP: [your local IP, e.g. 192.168.1.100]
- Save changes


===============================================================================
3. TOR INSTALLATION AND HIDDEN SERVICE CREATION
===============================================================================

TOR BROWSER INSTALLATION:

Step 1: Download Tor Browser
- Go to: https://www.torproject.org
- Download "Tor Browser for Windows"
- Install normally

Step 2: Run Tor Browser
- Launch Tor Browser
- Click "Connect"
- Wait for connection to Tor network
- LEAVE TOR BROWSER RUNNING (must run in background)

CREATING YOUR OWN ONION SEED NODE:

Step 1: Install Tor Expert Bundle (for advanced users)
- Go to: https://www.torproject.org/download/tor/
- Download "Expert Bundle for Windows"
- Extract to folder e.g: C:\Tor

Step 2: Configure Tor as Service
- Open Notepad as administrator
- Create file: C:\Tor\torrc
- Paste the following configuration:

SOCKSPort 9050
ControlPort 9051
HiddenServiceDir C:\Tor\rxb-hidden-service
HiddenServicePort 8327 127.0.0.1:8327

- Save file

Step 3: Run Tor
- Open CMD as administrator
- Navigate to folder: cd C:\Tor
- Run: tor.exe -f torrc
- Wait for message: "Bootstrapped 100%: Done"

Step 4: Read Your Onion Address
- Open file: C:\Tor\rxb-hidden-service\hostname
- Copy address (example: abc123xyz456.onion)
- THIS IS YOUR SEED NODE ONION!

Step 5: Add Your Seed Node to Configuration
- See section 4 below


===============================================================================
4. CREATING CONFIGURATION FILE
===============================================================================

CONFIGURATION FILE LOCATION:
C:\Users\[YOUR_NAME]\AppData\Roaming\RXBNetwork\rxb.conf

CREATING THE FILE:

Step 1: Open File Explorer
- Press Windows + R
- Type: %APPDATA%\RXBNetwork
- Click OK

Step 2: Create New File
- Right-click -> New -> Text Document
- Name it: rxb.conf (NOTE: remove .txt from the end!)

Step 3: Edit Configuration File
Open the file and paste example configuration:

# Basic RXBNetwork configuration

# P2P Port (default 8327)
port=8327

# Maximum connections
maxconnections=125

# Tor proxy (if using Tor)
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1

# Your own onion seed node (if you created one)
# externalip=[YOUR_ADDRESS].onion:8327

# Additional seed nodes (optional)
# addnode=m4gcv7eaorwu5nsmflfmeo6whrh5gu3wwkxfsanzs7evqaotolf4ksyd.onion:8327

# RPC (for advanced users)
server=1
rpcuser=rxbuser
rpcpassword=[SECURE_PASSWORD]
rpcallowip=127.0.0.1
rpcport=8328

# Optional: disk usage limitation (pruning)
# prune=5000

Save file and restart node.


===============================================================================
5. CREATING YOUR OWN ONION SEED NODE
===============================================================================

REQUIREMENTS:
- Tor Expert Bundle installed (see section 3)
- Port 8327 open in firewall (see section 2)
- Stable internet connection
- Computer running 24/7 (recommended)

SEED NODE CONFIGURATION:

Step 1: Make Sure Tor is Running
- Check if tor.exe is running
- Check file: C:\Tor\rxb-hidden-service\hostname
- Save your .onion address

Step 2: Configure rxb.conf
- Add to configuration file:

# Seed node configuration
externalip=[YOUR_ADDRESS].onion:8327
onion=127.0.0.1:9050
onlynet=onion
listen=1
discover=1

Step 3: Start Node
- Run rxbd.exe or rxb-qt.exe
- Node will automatically connect to network through Tor

Step 4: Check Status
- Open CMD
- Navigate to RXBNetwork installation folder
- Run: rxb-cli.exe getnetworkinfo
- Check if "localaddresses" contains your .onion address


===============================================================================
6. PASSWORDS AND SECURITY
===============================================================================

SECURE PASSWORD CREATION RULES:

FOR WALLET:
- Minimum 16 characters
- Upper and lowercase letters
- Numbers
- Special characters allowed: !@#$%^&*()_+=[]{}

Example: MyBr1xW@ll3t2025!Secure

FOR CONFIGURATION (rpcpassword):
When creating passwords for node configuration, you can use special characters 
and underscores. However, if you notice that special characters or underscores 
in passwords cause issues during configuration or node operation, we recommend 
creating a password consisting only of letters and numbers. Such simple 
passwords avoid potential compatibility problems in different system 
configurations.

- Use: letters and numbers
- Optional: underscore _ (if it doesn't cause problems)
- Minimum 20 characters

Example: RXBRPCPass2025SecureKey12345
Or with underscores: RXBRPC_Pass_2025_Secure_Key_12345

SECURE PASSWORD GENERATION:
1. Use online password generator (e.g. passwordsgenerator.net)
2. Length: 20-30 characters
3. Enable: uppercase, lowercase, numbers
4. For rpcpassword: disable special characters if causing problems
5. Save password in password manager (e.g. KeePass, Bitwarden)

WALLET BACKUP:
1. Close RXB
2. Navigate to: %APPDATA%\RXBNetwork
3. Copy wallet.dat file
4. Save on external drive, USB or cloud (encrypted!)
5. Backup monthly or after every important transaction


===============================================================================
7. CREATING CLEARNET-TOR BRIDGES
===============================================================================

WHAT IS A BRIDGE?
Bridge is a node that connects devices without Tor (clearnet) 
with nodes running through Tor (onion). Bridges are crucial 
for RXBNetwork operation.

HOW TO CREATE A BRIDGE?

Requirements:
- Public IP (static or dynamic DNS)
- Port 8327 open in router (port forwarding)
- Tor installed and configured
- Stable internet connection

BRIDGE CONFIGURATION:

Step 1: Configure Tor (see section 3)

Step 2: Configure rxb.conf as bridge:

# Clearnet-tor bridge configuration

# Listen on clearnet
bind=0.0.0.0:8327
listen=1

# Connect to Tor
proxy=127.0.0.1:9050
onion=127.0.0.1:9050

# Allow all networks
onlynet=ipv4
onlynet=onion

# Your public address (if you have static IP)
# externalip=[YOUR_PUBLIC_IP]:8327

# Or use domain (if you have DynamicDNS)
# externalip=yournode.ddns.net:8327

# Maximum connections
maxconnections=125

Step 3: Configure DynamicDNS (optional)
- Register at No-IP.com or DuckDNS.org (free)
- Create domain e.g: yourdomainnetwork.ddns.net
- Install DynamicDNS client to update IP
- Add to rxb.conf: externalip=yourdomainnetwork.ddns.net:8327

Step 4: Test Bridge
- Start node
- Wait 30 minutes
- Check logs for incoming connections


===============================================================================
8. SUPPORTING THE RXB
===============================================================================

RXBNetwork is decentralized and runs thanks to its users.
You can significantly support the network and increase its strength in the 
following ways:

RUNNING A NODE:
- Every active node increases network security
- The more nodes, the more resilient the network
- If possible, keep your node running 24/7

CREATING ONION SEED NODES:
- Onion nodes provide privacy and anonymity
- If you have stable connection, create your own seed node (see section 5)
- Every seed node helps new users connect to the network

CREATING BRIDGES:
- Bridges connect clearnet users with Tor network
- They are absolutely crucial for network operation
- If you have public IP, consider creating a bridge (see section 7)
- Bridges are the most valuable contribution to the network

MAINTAINING FULL NODE:
- Full node stores entire blockchain
- Helps new users synchronize
- Requires ~50 GB disk space
- In rxb.conf DO NOT add: prune=5000

HOW YOU CAN HELP:
1. Run a node and keep it online
2. If you have Tor - create onion seed node
3. If you have public IP - create clearnet-tor bridge
4. If you have plenty of space - run full node (without pruning)
5. Help other users in the community

EVERY NODE COUNTS!
Even if you can only keep your node running a few hours daily,
it still helps the network. You don't need any special skills
or expensive hardware. Your contribution is valuable!


===============================================================================
9. TROUBLESHOOTING
===============================================================================

Problem: Windows blocks execution
Solution:
1. Right-click on .exe file
2. Select "Properties"
3. In "General" tab check "Unblock"
4. Click "Apply" and "OK"
5. Run again

Problem: Antivirus blocks program
Solution:
1. Add RXBNetwork folder to antivirus exceptions
2. Temporarily disable antivirus during installation
3. Re-enable antivirus after installation

Problem: Wallet not synchronizing
Solution:
1. Check internet connection
2. Check if port 8327 is open (see section 2)
3. Wait longer (may take up to 60 minutes on first run)
4. Restart program
5. Check logs: %APPDATA%\RXBNetwork\debug.log

Problem: Tor not connecting
Solution:
1. Make sure Tor Browser is running BEFORE starting RXB
2. Check if rxb.conf has: proxy=127.0.0.1:9050
3. Check if Tor works: open https://check.torproject.org in Tor Browser
4. Restart Tor Browser and RXB

Problem: Cannot create onion seed node
Solution:
1. Check if tor.exe is running
2. Check if torrc file is properly configured
3. Check Tor logs: C:\Tor\tor.log
4. Make sure folder C:\Tor\rxb-hidden-service was created

Problem: Bridge not accepting connections
Solution:
1. Check if port 8327 is open in router (port forwarding)
2. Check public IP: https://whatismyipaddress.com
3. Add to rxb.conf: externalip=[YOUR_IP]:8327
4. Wait 30-60 minutes for node to be recognized in network

Problem: Forgot wallet password
Solution:
Unfortunately there is no way to recover password. If you don't have backup
of wallet.dat file, access to coins is lost forever.
ALWAYS BACKUP YOUR WALLET!


===============================================================================
IMPORTANT FILE LOCATIONS
===============================================================================

Data folder:
C:\Users\[YOUR_NAME]\AppData\Roaming\RXBNetwork\

Important files:
- wallet.dat           - Wallet (BACKUP!)
- rxb.conf     - Configuration
- debug.log            - Logs (helpful for problems)
- peers.dat            - List of known nodes
- blocks\              - Blockchain
- chainstate\          - Chain state


===============================================================================
PROJECT INFORMATION
===============================================================================

Website: https://rxbcore.org

Version: 0.1.0
Release date: January 2025
Technology: RXBNetwork utilizes Bitcoin Core 20.2 libraries and structure
Network: Mainnet


===============================================================================
SYSTEM REQUIREMENTS
===============================================================================

Minimum:
- System: Windows 7 SP1 (64-bit) or newer
- Processor: Dual-core 2.0 GHz
- RAM: 2 GB
- Disk: 10 GB free space
- Internet: Broadband connection

Recommended:
- System: Windows 10/11 (64-bit)
- Processor: Quad-core 2.5 GHz or better
- RAM: 4 GB or more
- Disk: SSD with 50 GB free space
- Internet: Broadband connection (unlimited data)

For bridges and seed nodes:
- Stable 24/7 connection
- Public IP or DynamicDNS
- No data transfer limit
- Minimum 100 GB free space


===============================================================================
LICENSE AND DISCLAIMERS
===============================================================================

RXBNetwork is open source software released under MIT license.

DISCLAIMER:
Software is provided "AS IS", without any warranties.
Authors are not responsible for any financial losses
or damages resulting from use of this software.

ALWAYS:
- Backup your wallet
- Use strong password
- Never share your password with anyone
- Don't install suspicious software
- Verify checksums of downloaded files


===============================================================================
CHECKSUMS
===============================================================================

Before running, verify that downloaded file is authentic.

MD5:
[generate: md5sum *.exe *.zip]

SHA256:
[generate: sha256sum *.exe *.zip]

How to check checksum in Windows:
1. Open PowerShell
2. Type: Get-FileHash .\rxb-0.1.0-win64-setup-unsigned.exe
3. Compare result with above

If checksums don't match - DO NOT RUN THE FILE!


===============================================================================

Thank you for using RXBNetwork!

Happy mining! Your contribution to maintaining nodes running in the background 
is invaluable. Every node strengthens the network. Especially valuable is 
running bridge nodes that connect clearnet with the onion network, integrating 
and unifying the entire RXBNetwork ecosystem.

===============================================================================