#include "ovat-log.h"
#include "PduR_CanNm.h"

void PduR_CanNmRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;

    OVAT_LOG(INFO, PDURSTUB, "stub PduR_CanNmRxIndication, RxPduId: %u, pdu: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
        RxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return;
}

void PduR_CanNmTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    OVAT_LOG(INFO, PDURSTUB, "stub PduR_CanNmTxConfirmation, TxPduId: %u, result: %u",
                TxPduId, result);
    return;
}

Std_ReturnType PduR_CanNmTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;

    OVAT_LOG(INFO, PDURSTUB, "stub PduR_CanNmTriggerTransmit, TxPduId: %u, pdu: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
        TxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return E_OK;

}

OVAT_LOG_REGISTER(pdurstub_logtype, PDURSTUB, INFO);

