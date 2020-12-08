#include "ovat-log.h"
#include "CanSM.h"

void CanSM_Init(void *config)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_Init");
    return;
}

void CanSM_MainFunction(void)
{

}

void CanSM_DeInit(void)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_DeInit");
    return;
}

OVAT_LOG_REGISTER(cansmstub_logtype, CANSMSTUB, INFO);

