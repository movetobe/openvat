#include "ovat-log.h"
#include "ComM.h"

void ComM_Init(void *config)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Init");
    return;
}

void ComM_MainFunction(void)
{
}

OVAT_LOG_REGISTER(commstub_logtype, COMMSTUB, INFO);

