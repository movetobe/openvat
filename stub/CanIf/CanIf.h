#ifndef CANIF_H
#define CANIF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
Std_ReturnType CanIf_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);
Std_ReturnType CanIf_RxIndication(PduIdType RxPduId, size_t length, void *data);

void CanIf_Init(void *config);
void CanIf_MainFunction(void);

#endif /* CANIF_H */
