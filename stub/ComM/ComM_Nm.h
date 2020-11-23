#ifndef COMM_NM_H
#define COMM_NM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

void ComM_Nm_NetworkStartIndication(NetworkHandleType Channel);
void ComM_Nm_NetworkMode(NetworkHandleType Channel);
void ComM_Nm_PrepareBusSleepMode(NetworkHandleType Channel);
void ComM_Nm_BusSleepMode(NetworkHandleType Channel);
void ComM_Nm_RestartIndication(NetworkHandleType Channel);

#endif /* COMM_NM_H */
