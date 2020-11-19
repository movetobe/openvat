#ifndef CANNM_H
#define CANNM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

typedef struct CanNm_ConfigType {
    int enabled;
} CanNm_ConfigType;

void CanNm_Init(const CanNm_ConfigType *cannmConfigPtr);
Std_ReturnType CanNm_NetworkRequest(NetworkHandleType nmChannelHandle);
Std_ReturnType CanNm_NetworkRelease(NetworkHandleType nmChannelHandle);
void CanNm_MainFunction(void);

#endif /* CANNM_H */
