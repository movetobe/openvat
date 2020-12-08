#ifndef DEM_H
#define DEM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"

typedef uint16 Dem_EventIdType;
typedef enum {
    DEM_EVENT_STATUS_PASSED = 0x00U,
    DEM_EVENT_STATUS_FAILED,
    DEM_EVENT_STATUS_PREPASSED,
    DEM_EVENT_STATUS_PREFAILED,
    DEM_EVENT_STATUS_FDC_THRESHOLD_REACHED,
} Dem_EventStatusType;
Std_ReturnType Dem_SetEventStatus(Dem_EventIdType EventId,
                                        Dem_EventStatusType EventStatus);

void Dem_Init(void *config);
void Dem_MainFunction(void);
void Dem_DeInit(void);

#endif /* DEM_H */
