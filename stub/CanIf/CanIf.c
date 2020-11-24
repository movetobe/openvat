#include "ovat-log.h"
#include "CanIf.h"

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    OVAT_LOG(INFO, CANIFSTUB, "stub CanIf_Transmit, TxPduId: %u, Pdu: 0x%02x %02x %02x %02x %02x %02x %02x %02x",
            TxPduId, pdu[0], pdu[1], pdu[2], pdu[3], pdu[4], pdu[5], pdu[6], pdu[7]);
    return E_OK;
}

OVAT_LOG_REGISTER(canifstub_logtype, CANIFSTUB, INFO);

