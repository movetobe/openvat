#ifndef NM_H
#define NM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "NmStack_types.h"

typedef struct Nm_ConfigType {
    int enabled;
} Nm_ConfigType;

void Nm_Init(const Nm_ConfigType *ConfigPtr);
Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_EnableCommunication(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_SetUserData(NetworkHandleType NetworkHandle,     const uint8* nmUserDataPtr);
Std_ReturnType Nm_GetUserData(NetworkHandleType NetworkHandle,     uint8* nmUserDataPtr);
Std_ReturnType Nm_GetPduData(NetworkHandleType NetworkHandle, uint8* nmPduData);
Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_GetNodeIdentifier(NetworkHandleType NetworkHandle,
                                            uint8* nmNodeIdPtr);

Std_ReturnType Nm_GetLocalNodeIdentifier(NetworkHandleType NetworkHandle,
                                                    uint8* nmNodeIdPtr);

Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType nmNetworkHandle,
                                                        boolean* nmRemoteSleepIndPtr);
Std_ReturnType Nm_GetState(NetworkHandleType nmNetworkHandle,
                            Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr);
void Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr);
void Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle);
void Nm_NetworkMode(NetworkHandleType nmNetworkHandle);
void Nm_BusSleepMode(NetworkHandleType nmNetworkHandle);
void Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle);
void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle);
void Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle);
void Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle);
void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle);
void Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle);
void Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle);
void Nm_PduRxIndication(NetworkHandleType nmNetworkHandle);
void Nm_StateChangeNotification(NetworkHandleType nmNetworkHandle,
                                    Nm_StateType nmPreviousState,
                                    Nm_StateType nmCurrentState);
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle);
void Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle);
void Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle);
void Nm_MainFunction(void);

#endif /* CANNM_H */
