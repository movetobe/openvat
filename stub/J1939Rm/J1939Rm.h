#ifndef J1939RM_H
#define J1939RM_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "StandardTypes.h"
#include "Nm.h"

typedef enum {
    J1939RM_EXTID_NONE = 0x00U,
    J1939RM_EXTID_ONE,
    J1939RM_EXTID_TWO,
    J1939RM_EXTID_THREE,
    J1939RM_EXTID_GF,
} J1939Rm_ExIdType;
typedef struct {
    J1939Rm_ExIdType extIdType;
    uint8 extId1;
    uint8 extId2;
    uint8 extId3;
} J1939Rm_ExtIdInfoType;

#endif /* J1939RM_H */
