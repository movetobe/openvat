#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#include <stdint.h>
#include "ComStackTypes.h"

typedef enum Std_Return {
    E_OK,
    E_NOT_OK
} Std_ReturnType;

typedef struct Std_VersionInfo {
    uint16 vendorID;
    uint16 moduleID;
    uint8 sw_major_version;
    uint8 sw_minor_version;
    uint8 sw_patch_version;
} Std_VersionInfoType;

#endif /* STANDARDTYPES_H */

