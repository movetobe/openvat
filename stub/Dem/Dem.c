#include "ovat-log.h"
#include "Dem.h"

Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId,
                                        Dem_EventStatusType EventStatus)


{
    OVAT_LOG(INFO, DEMSTUB, "stub Dem_SetEventStatus, EventId: %u, EventStatus: 0x%02x",
                EventId, EventStatus);
    return E_OK;
}

void Dem_Init(void *config)
{
    OVAT_LOG(INFO, DEMSTUB, "stub Det_Init");
    return;
}

void Dem_MainFunction(void)
{
}

void Dem_DeInit(void)
{
    OVAT_LOG(INFO, DEMSTUB, "stub Dem_DeInit");
    return;
}

OVAT_LOG_REGISTER(demstub_logtype, DEMSTUB, INFO);

