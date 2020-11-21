#ifndef DET_H
#define DET_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "NmStack_types.h"
Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId);
Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId);

#endif /* DET_H */
