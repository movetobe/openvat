#include "CanNm.h"
#include "ovat-log.h"
#include "StandardTypes.h"

void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_Init");
    return;
}

void CanNm_DeInit(void)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_DeInit");
    return;
}

Std_ReturnType CanNm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_PassiveStartUp, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_NetworkRequest, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_NetworkRelease, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_DisableCommunication(NetworkHandleType nmChannelHandle)

{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_DisableCommunication, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_EnableCommunication(NetworkHandleType nmChannelHandle)

{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_EnableCommunication, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_SetUserData(NetworkHandleType nmChannelHandle,
                                    const uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_SetUserData, nmChannelHandle: %u, userdata: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
                nmChannelHandle, nmUserDataPtr[0], nmUserDataPtr[1], nmUserDataPtr[2],
                nmUserDataPtr[3], nmUserDataPtr[4], nmUserDataPtr[5],
                nmUserDataPtr[6], nmUserDataPtr[7]);
    return E_OK;

}

Std_ReturnType CanNm_GetUserData(NetworkHandleType nmChannelHandle,     uint8* nmUserDataPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetUserData, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    OVAT_LOG(INFO, CANNMSTUB, "PduId: %u, PduInfo: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
            TxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return E_OK;
}

Std_ReturnType CanNm_GetNodeIdentifier(NetworkHandleType nmChannelHandle,
                                            uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetNodeIdentifier, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_GetLocalNodeIdentifier(NetworkHandleType nmChannelHandle,
                                                    uint8* nmNodeIdPtr)

{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetLocalNodeIdentifier, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_RepeatMessageRequest(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_RepeatMessageRequest, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_GetPduData(NetworkHandleType nmChannelHandle, uint8* nmPduData)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetPduData, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_GetState(NetworkHandleType nmChannelHandle,
                            Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetState, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

void CanNm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_GetVersionInfo\n");
}

Std_ReturnType CanNm_RequestBusSynchronization(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_RequestBusSynchronization, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_CheckRemoteSleepIndication(NetworkHandleType nmChannelHandle,
                                                        boolean* nmRemoteSleepIndPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_CheckRemoteSleepIndication, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_SetSleepReadyBit(NetworkHandleType nmChannelHandle,
                                            boolean nmSleepReadyBit)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_SetSleepReadyBit, nmChannelHandle: %u, SleepReadyBit: %u\n",
                nmChannelHandle, nmSleepReadyBit);
    return E_OK;
}

void CanNm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_TxConfirmation, TxPduId: %u, result: %u\n",
                TxPduId, result);
    return;
}

void CanNm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    OVAT_LOG(INFO, CANNMSTUB, "RxPduId: %u, PduInfo: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
            RxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return;
}

void CanNm_ConfirmPnAvailability(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_SetSleepReadyBit, nmChannelHandle: %u", nmChannelHandle);
    return;
}

Std_ReturnType CanNm_TriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    OVAT_LOG(INFO, CANNMSTUB, "TxPduId: %u, PduInfo: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
            TxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return E_OK;
}

void CanNm_MainFunction(void)
{
}

OVAT_LOG_REGISTER(cannmstub_logtype, CANNMSTUB, INFO);

