#include "ovat-log.h"
#include "ComM_Nm.h"

void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Nm_NetworkStartIndication, %u\n", Channel);
    return;
}

void ComM_Nm_NetworkMode(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Nm_NetworkMode, %u\n", Channel);
    return;
}

void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Nm_PrepareBusSleepMode, %u\n", Channel);
    return;
}

void ComM_Nm_BusSleepMode(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Nm_BusSleepMode, %u\n", Channel);
    return;
}

void ComM_Nm_RestartIndication(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, COMMSTUB, "stub ComM_Nm_RestartIndication, %u\n", Channel);
    return;
}

OVAT_LOG_REGISTER(commstub_logtype, COMMSTUB, INFO);

