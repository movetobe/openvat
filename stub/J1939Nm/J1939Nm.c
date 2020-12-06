#include "J1939Nm.h"
#include "ovat-log.h"
#include "StandardTypes.h"

void J1939Nm_Init(const J1939Nm_ConfigType *j1939nmConfigPtr)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_Init");
    return;
}

void J1939Nm_DeInit(void)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_DeInit");
    return;
}

Std_ReturnType J1939Nm_PassiveStartUp(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_PassiveStartUp, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType J1939Nm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_NetworkRequest, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType J1939Nm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_NetworkRelease, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType J1939Nm_GetState(NetworkHandleType nmChannelHandle,
                            Nm_StateType* nmStatePtr, Nm_ModeType* nmModePtr)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_GetState, nmChannelHandle: %u\n", nmChannelHandle);
    return E_OK;
}

void J1939Nm_GetBusOffDelay(NetworkHandleType network, uint8 *delayCyclesPtr)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_GetBusOffDelay, nmChannelHandle: %u, delayCyclesPtr: %u\n",
                                network, *delayCyclesPtr);
    return;
}


void J1939Nm_GetVersionInfo(Std_VersionInfoType* nmVerInfoPtr)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_GetVersionInfo\n");
}


void J1939Nm_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    OVAT_LOG(INFO, J1939NMSTUB, "stub J1939Nm_TxConfirmation, TxPduId: %u, result: %u\n",
                TxPduId, result);
    return;
}

void J1939Nm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    OVAT_LOG(INFO, J1939NMSTUB, "RxPduId: %u, PduInfo: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
            RxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return;
}

void J1939Nm_RequestIndication(uint8 node, NetworkHandleType channel,
                                uint32 requestedPgn, const J1939Rm_ExtIdInfoType* extIdInfo,
                                uint8 sourceAddress, uint8 destAddress,
                                uint8 priority)
{

}


void J1939Nm_MainFunction(void)
{
}

OVAT_LOG_REGISTER(j1939nmstub_logtype, J1939NMSTUB, INFO);

