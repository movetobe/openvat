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
void Can_DeInit(void);

Std_ReturnType Can_GetControllerTxErrorCounter(uint8 ControllerId,
                                                uint8* TxErrorCounterPtr);

Std_ReturnType Can_SetControllerMode(uint8 Controller,
                            Can_ControllerStateType Transition);
Std_ReturnType Can_CheckWakeup(uint8 Controller);

Std_ReturnType Can_SetBaudrate(uint8 Controller, uint16 BaudRateConfigID);

Std_ReturnType Can_SetIcomConfiguration(uint8 Controller,
                                            IcomConfigIdType ConfigurationId);



#endif /* CAN_H */
