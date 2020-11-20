#include "ovat-log.h"
#include "Nm.h"
#include "CanNm.h"

void Nm_Init(const Nm_ConfigType *ConfigPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_Init\n");
    return;
}

Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PassiveStartUp, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRequest, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRelease, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_DisableCommunication, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_EnableCommunication(NetworkHandleType NetworkHandle)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_EnableCommunication, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_SetUserData(NetworkHandleType NetworkHandle,
                                    const uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SetUserData, %u, userdata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                NetworkHandle, nmUserDataPtr[0], nmUserDataPtr[1], nmUserDataPtr[2],
                nmUserDataPtr[3], nmUserDataPtr[4], nmUserDataPtr[5],
                nmUserDataPtr[6], nmUserDataPtr[7]);
    return E_OK;

}

Std_ReturnType Nm_GetUserData(NetworkHandleType NetworkHandle,     uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetUserData, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetPduData(NetworkHandleType NetworkHandle, uint8* nmPduData)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetPduData, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RepeatMessageRequest, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetNodeIdentifier(NetworkHandleType NetworkHandle,
                                            uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetNodeIdentifier, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetLocalNodeIdentifier(NetworkHandleType NetworkHandle,
                                                    uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetLocalNodeIdentifier, %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType nmNetworkHandle,
                                                        boolean* nmRemoteSleepIndPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CheckRemoteSleepIndication, %u\n", nmNetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetState(NetworkHandleType nmNetworkHandle,
                            Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetState, %u\n", nmNetworkHandle);
    return E_OK;
}

void Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetVersionInfo\n");
}

void Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkStartIndication, %u\n", nmNetworkHandle);
}

void Nm_NetworkMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkMode, %u\n", nmNetworkHandle);
}

void Nm_BusSleepMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_BusSleepMode, %u\n", nmNetworkHandle);
}

void Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PrepareBusSleepMode, %u\n", nmNetworkHandle);
}

void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SynchronizeMode, %u\n", nmNetworkHandle);
}

void Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RemoteSleepIndication, %u\n", nmNetworkHandle);
}
void Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RemoteSleepIndication, %u\n", nmNetworkHandle);
}

void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SynchronizationPoint, %u\n", nmNetworkHandle);
}

void Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CoordReadyToSleepIndication, %u\n", nmChannelHandle);
}

void Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CoordReadyToSleepCancellation, %u\n", nmChannelHandle);
}

void Nm_PduRxIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PduRxIndication, %u\n", nmNetworkHandle);
}

void Nm_StateChangeNotification(NetworkHandleType nmNetworkHandle,
                                    Nm_StateType nmPreviousState,
                                    Nm_StateType nmCurrentState)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PduRxIndication, %u， prestate %u, currstate %u\n",
                nmNetworkHandle, nmPreviousState, nmCurrentState);
}
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RepeatMessageIndication, %u\n", nmNetworkHandle);
}

void Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_TxTimeoutException, %u\n", nmNetworkHandle);
}

void Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CarWakeUpIndication, %u\n", nmChannelHandle);
}

void Nm_MainFunction(void)
{
    OVAT_LOG(DEBUG, NMSTUB, "stub Nm_MainFunction\n");
}

OVAT_LOG_REGISTER(nmstub_logtype, ovat.nmstub, INFO);

