#ifndef __XBEE_H
#define __XBEE_H


enum estado { Waiting_Delimiter, Waiting_MSB, Waiting_LSB, Waiting_Data, Waiting_Checksum, Ready };
enum tipoTrama {  NonValid, Start, Stop, TransmitData, TransmitHistoryRequest, TransmitHistoryResponse,
        		  TransmitStatus, ATResponse, RemoteATResponse, UpdateRefreshTime, GotaDetectada, ActualizarVolumen,
        		  Reset
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


#endif /* end __XBEE_H */












