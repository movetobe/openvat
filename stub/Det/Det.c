#include "ovat-log.h"
#include "Det.h"

Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId)

{
    OVAT_LOG(INFO, DETSTUB, "stub Det_ReportError, ModuleId: 0x%02x, InstanceId: 0x%02x, ApiId: 0x%02x, ErrorId: 0x%02x",
                ModuleId, InstanceId, ApiId, ErrorId);
    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId,
                                    uint8 ApiId, uint8 ErrorId)

{
    OVAT_LOG(INFO, DETSTUB, "stub Det_ReportRuntimeError, ModuleId: 0x%02x, InstanceId: 0x%02x, ApiId: 0x%02x, ErrorId: 0x%02x",
                ModuleId, InstanceId, ApiId, ErrorId);
    return E_OK;
}

OVAT_LOG_REGISTER(detstub_logtype, DETSTUB, INFO);

