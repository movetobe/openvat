#include "ovat-log.h"
#include "CanIf.h"
#include "dynamic-string.h"
#include "Can.h"
#include "CanNm.h"

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    int i;
    struct ds s;
    Can_PduType can_pdu;

    ds_init(&s);
    ds_put_format(&s, "PduId: %u, ", TxPduId);
    ds_put_format(&s, "PduInfo: 0x");
    for (i = 0; i < PduInfoPtr->SduLength; i++) {
        ds_put_format(&s, "%02x", pdu[i]);
    }
    OVAT_LOG(INFO, CANIFSTUB, "stub CanIf_Transmit, %s", s.string);

    ds_destroy(&s);

    can_pdu.id = TxPduId;
    can_pdu.sdu = pdu;
    can_pdu.length = PduInfoPtr->SduLength;
    (void)Can_Write(TxPduId, &can_pdu);
    return E_OK;
}

Std_ReturnType CanIf_TxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    CanNm_TxConfirmation(TxPduId, E_OK);
    return E_OK;
}

Std_ReturnType CanIf_RxIndication(PduIdType RxPduId, size_t length, void *data)
{
    PduInfoType pduInfo;

    pduInfo.MetaDataPtr = NULL;
    pduInfo.SduDataPtr = data;
    pduInfo.SduLength = length;

    CanNm_RxIndication(RxPduId, &pduInfo);
    return E_OK;
}


OVAT_LOG_REGISTER(canifstub_logtype, CANIFSTUB, INFO);

