#ifndef CANSM_H
#define CANSM_H

#include "CanSM_CanIf.h"

void CanSM_Init(void *config);
void CanSM_MainFunction(void);
void CanSM_DeInit(void);

void CanSM_CurrentIcomConfiguration(uint8 ControllerId,
                                        IcomConfigIdType ConfigurationId,
                                        IcomSwitch_ErrorType Error);

#endif /* CANSM_H */
