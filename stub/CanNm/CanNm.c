#include "CanNm.h"

void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr)
{
    printf("stub CanNm_Init\n");
    return;
}

Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle)
{
    printf("stub CanNm_NetworkRequest, %u\n", nmChannelHandle);
    return E_OK;
}

Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle)
{
    printf("stub CanNm_NetworkRelease, %u\n", nmChannelHandle);
    return E_OK;
}

void CanNm_MainFunction(void)
{
    printf("stub CanNm_MainFunction\n");
}

