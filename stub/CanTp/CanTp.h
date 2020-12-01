#ifndef CANTP_H
#define CANTP_H

#include "ComStackTypes.h"
#include "StandardTypes.h"

typedef struct {
    uint8 config;
} CanTp_ConfigType;

void CanTp_Init(const CanTp_ConfigType* CfgPtr);
void CanTp_GetVersionInfo(Std_VersionInfoType* versioninfo);
void CanTp_Shutdown(void);
Std_ReturnType CanTp_Transmit(PduIdType TxPduId, const PduInfoType* PduInfoPtr);
Std_ReturnType CanTp_CancelTransmit(PduIdType TxPduId);
Std_ReturnType CanTp_CancelReceive(PduIdType RxPduId);
Std_ReturnType CanTp_ChangeParameter(PduIdType id, TPParameterType parameter, uint16 value);

Std_ReturnType CanTp_ReadParameter(PduIdType id, TPParameterType parameter, uint16* value);
void CanTp_MainFunction(void);
void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr);
void CanTp_TxConfirmation(PduIdType TxPduId, Std_ReturnType result);

#endif /* CANTP_H */
