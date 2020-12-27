#include "ovat-log.h"
#include "CanSM_CanIf.h"

void CanSM_TxTimeoutException(NetworkHandleType Channel)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_TxTimeoutException, Channel: %u\n", Channel);
    return;
}

void CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_CheckTransceiverWakeFlagIndication, Transceiver: %u\n", Transceiver);
    return;
}


void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_ClearTrcvWufFlagIndication, Transceiver: %u\n", Transceiver);
    return;
}


void CanSM_ConfirmPnAvailability(uint8 TransceiverId)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_ConfirmPnAvailability, TransceiverId: %u\n", TransceiverId);
    return;
}


void CanSM_ControllerBusOff(uint8 ControllerId)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_ControllerBusOff, ControllerId: %u\n", ControllerId);
    return;
}


void CanSM_ControllerModeIndication(uint8 ControllerId,
                                    Can_ControllerStateType ControllerMode)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_ControllerModeIndication, ControllerId: %u, ControllerMode: %u\n",
                    ControllerId, ControllerMode);
    return;
}

void CanSM_TransceiverModeIndication(uint8 TransceiverId,
                                    CanTrcv_TrcvModeType TransceiverMode)
{
    OVAT_LOG(INFO, CANSMSTUB, "stub CanSM_TransceiverModeIndication, TransceiverId: %u, TransceiverMode: %u\n",
                    TransceiverId, TransceiverMode);
    return;
}

