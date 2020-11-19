#ifndef STANDARDTYPES_H
#define STANDARDTYPES_H

#include <stdint.h>
#include "ComStackTypes.h"

typedef enum Std_Return {
    E_OK,
    E_NOT_OK
} Std_ReturnType;

typedef struct Std_VersionInfo {
    uint8_t major_version;
    uint8_t minor_version;
} Std_VersionInfoType;

#endif /* STANDARDTYPES_H */

