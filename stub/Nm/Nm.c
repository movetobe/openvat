#include "ovat-log.h"
#include "Nm.h"
#include "CanNm.h"

void Nm_Init(const Nm_ConfigType *ConfigPtr)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_Init\n");
    return;
}

Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRequest, %u\n", NetworkHandle);
    CanNm_NetworkRequest(NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_NetworkRelease, %u\n", NetworkHandle);
    CanNm_NetworkRelease(NetworkHandle);
    return E_OK;
}
void Nm_MainFunction(void)
{
    OVAT_LOG(INFO, NMSTUB, "stub Nm_MainFunction\n");
}

OVAT_LOG_REGISTER(nmstub_logtype, ovat.nmstub, INFO);


