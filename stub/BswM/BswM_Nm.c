#include "ovat-log.h"
#include "BswM_Nm.h"

void BswM_Nm_CarWakeUpIndication(NetworkHandleType Network)
{
    OVAT_LOG(INFO, BSWMSTUB, "stub BswM_Nm_CarWakeUpIndication %u\n", Network);
    return;
}

OVAT_LOG_REGISTER(bswmstub_logtype, BSWMSTUB, INFO);

