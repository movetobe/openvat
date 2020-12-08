#ifndef CANSM_H
#define CANSM_H

#include "CanSM_CanIf.h"

void CanSM_Init(void *config);
void CanSM_MainFunction(void);
void CanSM_DeInit(void);

#endif /* CANSM_H */
