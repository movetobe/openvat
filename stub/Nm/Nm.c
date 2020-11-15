#include "Nm.h"
#include "CanNm.h"

void Nm_Init(const Nm_ConfigType *ConfigPtr)
{
    printf("stub Nm_Init\n");
    return;
}

Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle)
{
    printf("stub Nm_NetworkRequest, %u\n", NetworkHandle);
    CanNm_NetworkRequest(NetworkHandle);
    return E_OK;
}

Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle)
{
    printf("stub Nm_NetworkRelease, %u\n", NetworkHandle);
    CanNm_NetworkRelease(NetworkHandle);
    return E_OK;
}
void Nm_MainFunction(void)
{
    printf("stub Nm_MainFunction\n");
}

