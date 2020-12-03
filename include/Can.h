#ifndef CAN_H
#define CAN_H

#include "StandardTypes.h"
#include "Can_GeneralTypes.h"

typedef uint32 Can_IdType;
typedef uint8 Can_HwHandleType; 

typedef struct {
    uint8 config;
} Can_ConfigType;

 typedef struct {
     PduIdType swPduHandle;
     uint8 length;
     Can_IdType id;
     uint8 *sdu;
} Can_PduType;

int can_create_vcan_dev(const char *device);
int can_remove_vcan_dev(const char *device);
int can_destroy_vcan_dev(void);
Std_ReturnType Can_Write(Can_HwHandleType Hth, const Can_PduType* PduInfo);
void Can_Init(const Can_ConfigType *config);
void Can_MainFunction(void);

#endif /* CAN_H */
