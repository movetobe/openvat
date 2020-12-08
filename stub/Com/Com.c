#include "ovat-log.h"
#include "Com.h"

uint8 Com_SendSignal(Com_SignalIdType SignalId, const void* SignalDataPtr)
{
    uint8 *data = (uint8 *)SignalDataPtr;
    OVAT_LOG(INFO, COMSTUB, "stub Com_SendSignal, SignalId: %u, Data: 0x%02x", SignalId, *data);
    return E_OK;
}

void Com_Init(void *config)
{
    OVAT_LOG(INFO, COMSTUB, "stub Com_Init");
    return;
}

void Com_MainFunction(void)
{
}

void Com_DeInit(void)
{
    OVAT_LOG(INFO, COMSTUB, "stub Com_DeInit");
    return;
}

OVAT_LOG_REGISTER(comstub_logtype, COMSTUB, INFO);

