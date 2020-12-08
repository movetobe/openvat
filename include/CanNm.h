#ifndef CANNM_H
#define CANNM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "Nm.h"

typedef struct CanNm_ConfigType {
    int enabled;
} CanNm_ConfigType;

void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr);
void CanNm_DeInit(void);
Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle,
                                  const uint8* nmUserDataPtr);
Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle,     uint8* nmUserDataPtr);
Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle,
                                          uint8* nmNodeIdPtr);
Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle,
                                                   uint8* nmNodeIdPtr);
Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduData);
Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle,
                           Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);
void CanNm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr);
Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle,
                                                       boolean* nmRemoteSleepIndPtr);
Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle,
                                           boolean nmSleepReadyBit);
void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);
void CanNm_MainFunction(void);

#endif /* CANNM_H */
