#ifndef COMSTACKTYPES_H
#define COMSTACKTYPES_H

#include <stdint.h>

typedef uint8_t NetworkHandleType;

#ifndef boolean
typedef uint8_t boolean;
#endif

#ifndef uint8
typedef uint8_t uint8;
#endif

#ifndef uint16
typedef uint16_t uint16;
#endif


#ifndef PduIdType
typedef uint16_t PduIdType;
#endif

#ifndef PduLengthType
typedef uint32_t PduLengthType;
#endif

typedef struct PduInfo {
    uint8 *SduDataPtr;
    uint8 *MetaDataPtr;
    PduLengthType SduLength;
} PduInfoType;
#endif /* COMSTACKTYPES_H */

