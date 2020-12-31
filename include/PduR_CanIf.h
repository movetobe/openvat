#ifndef PDUR_CANIF_H
#define PDUR_CANIF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

void PduR_CanIfRxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void PduR_CanIfTxConfirmation(PduIdType TxPduId, Std_ReturnType result);

#endif /* PDUR_CANIF_H */
