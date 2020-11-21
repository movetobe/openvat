#ifndef CANIF_H
#define CANIF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "NmStack_types.h"

Std_ReturnType CanIf_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);

#endif /* CANIF_H */
