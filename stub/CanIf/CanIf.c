#include "ovat-log.h"
#include "CanIf.h"
#include "dynamic-string.h"
#include "Can.h"
#include "CanNm.h"
#include "ovat-list.h"

static struct list_head txpduid_list;
struct pduid_trans {
    uint32_t sw;
    uint32_t hw;
    struct list_head list_node;
};

static uint32_t canif_sw2hw(uint32_t txpduid)
{
    struct pduid_trans *node = NULL;

    if (list_empty(&txpduid_list)) {
        return -1;
    }

    list_for_each_entry(node, &txpduid_list, list_node) {
        if ((node != NULL) && (node->sw == txpduid)) {
            return node->hw;
        }
    }
    return -1;
}

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr)
{
    uint8 *pdu = PduInfoPtr->SduDataPtr;
    int i;
    struct ds s;
    Can_PduType can_pdu;
    uint32_t hw;
    struct pduid_trans *new_trans = NULL;

    ds_init(&s);
    ds_put_format(&s, "PduId: %u, ", TxPduId);
    if (PduInfoPtr->MetaDataPtr != NULL) {
        ds_put_format(&s, "MetaData: 0x%02x", PduInfoPtr->MetaDataPtr[0]);
    }
    ds_put_format(&s, "PduInfo: 0x");
    for (i = 0; i < PduInfoPtr->SduLength; i++) {
        ds_put_format(&s, "%02x", pdu[i]);
    }
    OVAT_LOG(INFO, CANIFSTUB, "stub CanIf_Transmit, %s", s.string);

    ds_destroy(&s);

    can_pdu.swPduHandle = TxPduId;
    can_pdu.id = TxPduId;
    can_pdu.sdu = pdu;
    can_pdu.length = PduInfoPtr->SduLength;
    hw = canif_sw2hw(TxPduId);
    if (hw == -1) {
        hw = list_size(&txpduid_list);
        new_trans = calloc(1, sizeof(struct pduid_trans));
        new_trans->hw = hw;
        new_trans->sw = TxPduId;
        list_add_tail(&new_trans->list_node, &txpduid_list);
    }
    (void)Can_Write(hw, &can_pdu);
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

void CanIf_Init(void *config)
{
    OVAT_LOG(INFO, CANIFSTUB, "stub CanIf_Init");
    INIT_LIST_HEAD(&txpduid_list);
    return;
}

void CanIf_MainFunction(void)
{

}

void CanIf_DeInit(void)
{
    struct pduid_trans *trans, *ttrans;
    list_for_each_entry_safe(trans, ttrans, &txpduid_list, list_node) {
        list_del(&trans->list_node);
        free(trans);
    }
    OVAT_LOG(INFO, CANIFSTUB, "stub CanIf_DeInit");
    return;
}


OVAT_LOG_REGISTER(canifstub_logtype, CANIFSTUB, INFO);

