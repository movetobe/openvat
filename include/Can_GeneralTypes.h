#ifndef CAN_GENERALTYPES_H
#define CAN_GENERALTYPES_H

typedef enum {
    CAN_CS_UNINIT  = 0x00, /* CAN controller state UNINIT. */
    CAN_CS_STARTED = 0x01, /* CAN controller state STARTED. */
    CAN_CS_STOPPED = 0x02, /* CAN controller state STOPPED. */
    CAN_CS_SLEEP   = 0x03, /* CAN controller state SLEEP. */
} Can_ControllerStateType;

typedef enum {
    CANTRCV_TRCVMODE_SLEEP,          /* Transceiver mode SLEEP */
    CANTRCV_TRCVMODE_STANDBY,        /* Transceiver mode STANDBY */
    CANTRCV_TRCVMODE_NORMAL = 0x00,  /* Transceiver mode NORMAL */
} CanTrcv_TrcvModeType;

#endif
