#include "BswM_J1939Nm.h"
#include "ovat-log.h"

void BswM_J1939Nm_StateChangeNotification(NetworkHandleType Network,
                                            uint8 Node, Nm_StateType NmState)
{
    OVAT_LOG(INFO, BSWMSTUB, "stub BswM_J1939Nm_StateChangeNotification, Network: %u \
                             node: %u, NmState: %u", Network, Node, NmState);
    return;
}

