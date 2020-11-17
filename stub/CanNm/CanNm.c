#include "CanNm.h"
#include "ovat-log.h"

void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_Init");
    return;
}

Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_NetworkRequest, %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_NetworkRelease, %u\n", nmChannelHandle);
    return E_OK;
}

void CanNm_MainFunction(void)
{
    OVAT_LOG(INFO, CANNMSTUB, "stub CanNm_MainFunction\n");
}

OVAT_LOG_REGISTER(cannmstub_logtype, ovat.cannmstub, INFO);

