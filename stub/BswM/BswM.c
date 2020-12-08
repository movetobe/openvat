#include "ovat-log.h"
#include "BswM.h"

void BswM_Init(const void *config)
{
    OVAT_LOG(INFO, BSWMSTUB, "stub BswM_Init");
    return;
}

void BswM_MainFunction(void)
{
}

void BswM_DeInit(void)
{
    OVAT_LOG(INFO, BSWMSTUB, "stub BswM_DeInit");
    return;
}

OVAT_LOG_REGISTER(bswmstub_logtype, BSWMSTUB, INFO);

