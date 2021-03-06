#ifndef __AUX_H
#define __AUX_H

#include <stdint.h>
#include "type.h"
#include "ff.h"
#include "xbee.h"

extern volatile float volumenPrecipitacion;
extern volatile float intensidadaPrecipitacion;
extern volatile int32_t updateTimeXBEE;  //Tiempo de Update de Datos en segundos
extern volatile int32_t updateTimeRS485;  //Tiempo de Update de Datos en segundos
extern volatile int32_t updateTimeSD;
extern volatile float superficieSensor;      //en mm2
extern volatile int8_t transmitirXBEE;
extern volatile int8_t transmitirRS485;
extern volatile int8_t saveSD;
extern volatile int8_t startReceived;
extern volatile int8_t startReceivedUART;
extern volatile int8_t stopReceived;
extern volatile int8_t stopReceivedUART;
extern volatile int8_t rtcReadRequestReceived;
extern volatile int8_t rtcReadRequestReceivedUART;
extern volatile float lastSendVolume;
extern volatile int32_t elapsedTimeSinceLastVolumeUpdate;
extern volatile FIL fileGotas, filePrecipitacion, fileBackupGotas, fileBackupPrecipitacion;
extern volatile FIL * ptrFileBackup;

extern void ResetMediciones();
extern void CalcularVolumen (float maximo);
extern void UpdatePrecipitationDataUserInterface (int8_t UART);
extern void UpdatePrecipitationDataSD(FIL *fp);
extern void SaveDropDataSD(FIL *fp, float tamanioGota);
void TransferirDatosSDtoUserInterface(FIL *fp, int8_t UART, enum tipoTrama tramaType);

#endif
