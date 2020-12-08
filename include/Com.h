#ifndef COM_H
#define COM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

#ifndef Com_SignalIdType
typedef uint16_t Com_SignalIdType;
#endif

uint8 Com_SendSignal(Com_SignalIdType SignalId, const void* SignalDataPtr);
void Com_Init(void *config);
void Com_MainFunction(void);
void Com_DeInit(void);

#endif /* COM_H */
