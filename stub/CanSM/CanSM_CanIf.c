#include "ovat-log.h"
#include "CanSM_CanIf.h"

void CanSM_TxTimeoutException(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_TxTimeoutException, %u\n", Channel);
    return;
}

OVAT_LOG_REGISTER(cansmstub_logtype, ovat.cansmstub, INFO);

