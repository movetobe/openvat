Open Virtual AUTOSAR TestBed.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
ovat-appctl <--> ovat-core

ovat-appctl and ovat-core are 2 process, they communicate with unix sock

ovat-appctl: input commands and trans to ovat-core
ovat-core: process all commands

For example:
AUTOSAR Nm/CanNm module
1. ovat-appctl module/load Nm
2. ovat-appctl module/load CanNm
3. ovat-appctl nm/network-request 1 /* The same as call Nm_NetworkRequest(1) */
4. CanNm shall goto normal operation, and shall 'send' Can PDUs
5. ovat-appctl nm/network-release 1 /* The same as call Nm_NetworkRelease(1) */
6. CanNm shall stop 'send' Can PDUs and goto sleep mode.

movetobe@outlook.com
2020/10/29
