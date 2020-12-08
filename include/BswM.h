#ifndef BSWM_H
#define BSWM_H

#include "BswM_Nm.h"
#include "BswM_J1939Nm.h"

void BswM_Init(const void *config);
void BswM_MainFunction(void);
void BswM_DeInit(void);

#endif /* BSWM_H */
