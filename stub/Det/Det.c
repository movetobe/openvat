#include "ovat-log.h"
#include "Det.h"

Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId)

{
    OVAT_LOG(INFO, DETSTUB, "stub Det_ReportError, ModuleId: %u, InstanceId: %u, ApiId: %u, ErrorId: %u",
                ModuleId, InstanceId, ApiId, ErrorId);
    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId)

{
    OVAT_LOG(INFO, DETSTUB, "stub Det_ReportRuntimeError, ModuleId: %u, InstanceId: %u, ApiId: %u, ErrorId: %u",
                ModuleId, InstanceId, ApiId, ErrorId);
    return E_OK;
}

OVAT_LOG_REGISTER(detstub_logtype, DETSTUB, INFO);

