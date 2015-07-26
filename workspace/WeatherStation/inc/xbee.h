#ifndef __XBEE_H
#define __XBEE_H


enum estado { Waiting_Delimiter, Waiting_MSB, Waiting_LSB, Waiting_Data, Waiting_Checksum, Ready };
enum tipoTrama {  NonValid, Start = 0x10, StartOk = 0x11, Stop = 0x90, StopOk = 0x91, TransmitData, TransmitStatus, ATResponse, RemoteATResponse,
    			  GotaDetectada = 0x12, ActualizarVolumen = 0x13, ResetMedicionesLluvia = 0x14, UpdateRefreshTime = 0x15,
    			  TransmitHistoryGotaDetectada = 0x16, TransmitHistoryVolumenIntesidad = 0x17, FinishHistoryTransmission = 0x18,
    			  RTCReadRequest = 0x20, RTCReadResponse = 0x21, RTCSet = 0x22
    		   };

extern enum estado estadoActual;
extern enum tipoTrama tipoTramaRecibida;
extern volatile uint8_t xbeeFrame[60];
extern volatile uint16_t currentIndex;
extern volatile uint16_t frameDataLength;
extern volatile uint16_t dataArrived;

extern void CargarByte(volatile uint8_t byteReceived);
extern void BorrarDatosFrame();
extern enum tipoTrama ChequearTrama();
extern void EnviarStartOk(int8_t UART);
extern void EnviarFinishHistoryTransmission(int8_t UART);
extern void EnviarStopOk(int8_t UART);
extern void EnviarRTCReadResponse(int8_t UART);
extern void SetearRTC(int8_t dia, int8_t mes, int16_t anio, int8_t hora, int8_t minuto, int8_t segundo);

#endif /* end __XBEE_H */












