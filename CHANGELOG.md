
### RXB Core Changelog


### v0.3.0 Foxbat - April 2026


### Consensus



The LWMA per-block difficulty algorithm has been removed entirely and
replaced with the classical Bitcoin-style retarget mechanism operating
over a window of 2016 blocks.

The asymmetric clamp is applied at each retarget interval.
Difficulty may increase by no more than two percent per window. 
Difficulty may decrease by up to fifty five percent per window. 
This arrangement makes hashrate-rental attacks economically unviable. 
To double the difficulty an attacker would need to sustain their hashrate continuously for
roughly 490 days across 35 retarget windows. 
Should hashrate fall the network recovers within a single retarget interval.

The target block spacing remains ten minutes and the retarget interval is now 2016 blocks, consistent with Bitcoin.


### Security
 
Under the previous per-block scheme a rented ASIC farm could spike difficulty in
minutes. Under the new scheme the same attack would require sustained
expenditure measured in years.

### Upgrade Notice

This release contains consensus critical changes. 
All node operators must upgrade from source code immediately. 
Nodes running v0.2.0 or earlier will not be compatible with the updated network rules.

Source: https://github.com/Heiwabitnull/rxb-core
I hope it will help and allows farms and home miners with ACSI/GPU devices to use RXBcore sustainably.



## v0.2.0 - April 2026

Asymmetric LWMA clamp introduced. 
Solvetime ceiling raised. Experimental label removed. Pool support added.

## v0.1.0 - February 2026

Initial release of RXB Core.
