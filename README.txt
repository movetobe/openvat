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

NOTE: ovat-appctl commands/list can show all command supported

All modules are stub. If you want to changed to real module, you should provide the module's .so,
and export LD_PRELOAD environment. For example: export LD_PRELOAD=/usr/lib/libcannm.so before you
start ovat-core. unset LD_PRELOAD if necessary. These operations can be done in ovat-core.service,
you can add MODULES_SO_PATH in the ovat-core.service script file.

movetobe@outlook.com
2020/10/29
