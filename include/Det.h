#ifndef DET_H
#define DET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId);
Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId);
void Det_Init(void *config);
void Det_MainFunction(void);
void Det_DeInit(void);

#endif /* DET_H */
