#ifndef CANSM_CANIF_H
#define CANSM_CANIF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ComStackTypes.h"
#include "StandardTypes.h"
#include "Can_GeneralTypes.h"

void CanSM_TxTimeoutException(NetworkHandleType Channel);

void CanSM_CheckTransceiverWakeFlagIndication(uint8 Transceiver);

void CanSM_ClearTrcvWufFlagIndication(uint8 Transceiver);

void CanSM_ConfirmPnAvailability(uint8 TransceiverId);

void CanSM_ControllerBusOff(uint8 ControllerId);

void CanSM_ControllerModeIndication(uint8 ControllerId,
                                    Can_ControllerStateType ControllerMode);

void CanSM_TransceiverModeIndication(uint8 TransceiverId,
                                    CanTrcv_TrcvModeType TransceiverMode);

#endif /* CANSM_CANIF_H */
