#include "ovat-log.h"
#include "Nm.h"
#include "CanNm.h"

char *nmStates[] = {
    [NM_STATE_UNINIT] = "NM_STATE_UNINIT",
    [NM_STATE_BUS_SLEEP] = "NM_STATE_BUS_SLEEP",
    [NM_STATE_PREPARE_BUS_SLEEP] = "NM_STATE_PREPARE_BUS_SLEEP",
    [NM_STATE_READY_SLEEP] = "NM_STATE_READY_SLEEP",
    [NM_STATE_NORMAL_OPERATION] = "NM_STATE_NORMAL_OPERATION",
    [NM_STATE_REPEAT_MESSAGE] = "NM_STATE_REPEAT_MESSAGE",
    [NM_STATE_SYNCHRONIZE] = "NM_STATE_SYNCHRONIZE",
    [NM_STATE_OFFLINE] = "NM_STATE_OFFLINE",
};
char *nmModes[] = {
    [NM_MODE_BUS_SLEEP] = "NM_MODE_BUS_SLEEP",
    [NM_MODE_PREPARE_BUS_SLEEP] = "NM_MODE_PREPARE_BUS_SLEEP",
    [NM_MODE_SYNCHRONIZE] = "NM_MODE_SYNCHRONIZE",
    [NM_MODE_NETWORK] = "NM_MODE_NETWORK",
};

void Nm_Init(const Nm_ConfigType *ConfigPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_Init\n");
    return;
}

Std_ReturnType Nm_PassiveStartUp(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PassiveStartUp, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRequest, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRelease, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_DisableCommunication(NetworkHandleType NetworkHandle)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_DisableCommunication, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_EnableCommunication(NetworkHandleType NetworkHandle)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_EnableCommunication, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_SetUserData(NetworkHandleType NetworkHandle,
                                    const uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SetUserData, NetworkHandle: %u, userdata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                NetworkHandle, nmUserDataPtr[0], nmUserDataPtr[1], nmUserDataPtr[2],
                nmUserDataPtr[3], nmUserDataPtr[4], nmUserDataPtr[5],
                nmUserDataPtr[6], nmUserDataPtr[7]);
    return E_OK;

}

Std_ReturnType Nm_GetUserData(NetworkHandleType NetworkHandle,     uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetUserData, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetPduData(NetworkHandleType NetworkHandle, uint8* nmPduData)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetPduData, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_RepeatMessageRequest(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RepeatMessageRequest, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetNodeIdentifier(NetworkHandleType NetworkHandle,
                                            uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetNodeIdentifier, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetLocalNodeIdentifier(NetworkHandleType NetworkHandle,
                                                    uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetLocalNodeIdentifier, NetworkHandle: %u\n", NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_CheckRemoteSleepIndication(NetworkHandleType nmNetworkHandle,
                                                        boolean* nmRemoteSleepIndPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CheckRemoteSleepIndication, NetworkHandle: %u\n", nmNetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_GetState(NetworkHandleType nmNetworkHandle,
                            Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetState, NetworkHandle: %u\n", nmNetworkHandle);
    return E_OK;
}

void Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_GetVersionInfo\n");
}

void Nm_NetworkStartIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkStartIndication, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_NetworkMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkMode, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_BusSleepMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_BusSleepMode, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_PrepareBusSleepMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PrepareBusSleepMode, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_SynchronizeMode(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SynchronizeMode, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_RemoteSleepIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RemoteSleepIndication, NetworkHandle: %u\n", nmNetworkHandle);
}
void Nm_RemoteSleepCancellation(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RemoteSleepIndication, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_SynchronizationPoint(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_SynchronizationPoint, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_CoordReadyToSleepIndication(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CoordReadyToSleepIndication, NetworkHandle: %u\n", nmChannelHandle);
}

void Nm_CoordReadyToSleepCancellation(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CoordReadyToSleepCancellation, NetworkHandle: %u\n", nmChannelHandle);
}

void Nm_PduRxIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PduRxIndication, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_StateChangeNotification(NetworkHandleType nmNetworkHandle,
                                    Nm_StateType nmPreviousState,
                                    Nm_StateType nmCurrentState)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_PduRxIndication, NetworkHandle: %u， prestate %s, currstate %s\n",
                nmNetworkHandle, nmStates[nmPreviousState], nmStates[nmCurrentState]);
}
void Nm_RepeatMessageIndication(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_RepeatMessageIndication, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_TxTimeoutException(NetworkHandleType nmNetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_TxTimeoutException, NetworkHandle: %u\n", nmNetworkHandle);
}

void Nm_CarWakeUpIndication(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_CarWakeUpIndication, NetworkHandle: %u\n", nmChannelHandle);
}

void Nm_MainFunction(void)
{
}

OVAT_LOG_REGISTER(nmstub_logtype, NMSTUB, INFO);

