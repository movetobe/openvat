#ifndef PDUR_CANNM_H
#define PDUR_CANNM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

void PduR_CanNmRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void PduR_CanNmTxConfirmation(PduIdType TxPduId, Std_ReturnType result);
Std_ReturnType PduR_CanNmTriggerTransmit(PduIdType TxPduId, PduInfoType* PduInfoPtr);

#endif /* PDUR_CANNM_H */
