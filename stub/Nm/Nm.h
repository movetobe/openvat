#ifndef NM_H
#define NM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Common_Type.h"

typedef struct Nm_ConfigType {
    int enabled;
} Nm_ConfigType;

void Nm_Init(const Nm_ConfigType *ConfigPtr);
Std_ReturnType Nm_NetworkRequest(NetworkHandleType NetworkHandle);
Std_ReturnType Nm_NetworkRelease(NetworkHandleType NetworkHandle);
void Nm_MainFunction(void);

#endif /* CANNM_H */
