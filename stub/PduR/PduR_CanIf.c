#include "ovat-log.h"
#include "PduR_CanIf.h"

void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;

    OVAT_LOG(INFO, PDURSTUB, "stub PduR_CanIfRxIndication, RxPduId: %u, pdu: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
        RxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return;
}

void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result)
{
    OVAT_LOG(INFO, PDURSTUB, "stub PduR_CanIfTxConfirmation, TxPduId: %u, result: %u",
                TxPduId, result);
    return;
}

