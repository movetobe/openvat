#include "ovat-log.h"
#include "PduR.h"

void PduR_Init(void *config)
{
    OVAT_LOG(INFO, PDURSTUB, "stub PduR_Init");
    return;
}

void PduR_MainFunction(void)
{
}

OVAT_LOG_REGISTER(pdurstub_logtype, PDURSTUB, INFO);

