#include "CanTp.h"
#include "ovat-log.h"
#include "StandardTypes.h"
#include "dynamic-string.h"

void CanTp_Init(const CanTp_ConfigType* CfgPtr)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_Init");
    return;
}

void CanTp_GetVersionInfo(Std_VersionInfoType* versioninfo)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_GetVersionInfo");
    return;
}
void CanTp_Shutdown(void)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_Shutdown");
    return;
}
Std_ReturnType CanTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    int i;
    struct ds s;

    ds_init(&s);
    ds_put_format(&s, "PduId: %u, ", TxPduId);
    ds_put_format(&s, "PduInfo: 0x");
    for (i = 0; i < PduInfoPtr->SduLength; i++) {
        ds_put_format(&s, "%02x", pdu[i]);
    }
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_Transmit, %s", s->string);

    ds_destroy(&s);
    return E_OK;
}
Std_ReturnType CanTp_CancelTransmit(PduIdType TxPduId)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_CancelTransmit, TxPduId: %u\n", TxPduId);
    return E_OK;
}
Std_ReturnType CanTp_CancelReceive(PduIdType RxPduId)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_CancelReceive, RxPduId: %u\n", RxPduId);
    return E_OK;
}

Std_ReturnType CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_ChangeParameter, id: %u, parameter: %u, value: %u\n",
                        id, parameter, value);
    return E_OK;
}

Std_ReturnType CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_ReadParameter, id: %u, parameter: %u, value: %u\n",
                        id, parameter, *value);
    return E_OK;

}
void CanTp_MainFunction(void)
{
}
void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    int i;
    struct ds s;

    ds_init(&s);
    ds_put_format(&s, "PduId: %u, ", RxPduId);
    ds_put_format(&s, "PduInfo: 0x");
    for (i = 0; i < PduInfoPtr->SduLength; i++) {
        ds_put_format(&s, "%02x", pdu[i]);
    }
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_RxIndication, %s", s->string);

    ds_destroy(&s);
    return E_OK;
}
void CanTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    OVAT_LOG(INFO, CANTPSTUB, "stub CanTp_TxConfirmation, TxPduId: %u, result: %u",
                        TxPduId, result);
    return;

}

OVAT_LOG_REGISTER(cantpstub_logtype, CANTPSTUB, INFO);

