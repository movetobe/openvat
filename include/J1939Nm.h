#ifndef J1939NM_H
#define J1939NM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "J1939Rm.h"
#include "Nm.h"

typedef struct J1939Nm_ConfigType {
    int enabled;
} J1939Nm_ConfigType;

void J1939Nm_Init(const J1939Nm_ConfigType *j1939nmConfigPtr);
void J1939Nm_DeInit(void);
Std_ReturnType J1939Nm_PassiveStartUp(NetworkHandleType nmChannelHandle);
Std_ReturnType J1939Nm_NetworkRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType J1939Nm_NetworkRelease(NetworkHandleType nmChannelHandle);
Std_ReturnType J1939Nm_GetState(NetworkHandleType nmChannelHandle,
                           Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);
void J1939Nm_GetBusOffDelay(NetworkHandleType network, uint8 *delayCyclesPtr);
void J1939Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr);
void J1939Nm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
void J1939Nm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void J1939Nm_RequestIndication(uint8 node, NetworkHandleType channel,
                                uint32 requestedPgn, const J1939Rm_ExtIdInfoType* extIdInfo,
                                uint8 sourceAddress, uint8 destAddress,
                                uint8 priority);
void J1939Nm_MainFunction(void);

#endif /* J1939NM_H */
