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

void CanSM_CurrentIcomConfiguration(uint8 ControllerId,
                                        IcomConfigIdType ConfigurationId,
                                        IcomSwitch_ErrorType Error)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_CurrentIcomConfiguration, ControllerId: %u, ConfigurationId: %u, Error: %u",
                    ControllerId, ConfigurationId, Error);
    return;
}

OVAT_LOG_REGISTER(cansmstub_logtype, CANSMSTUB, INFO);

