#include "LPC17xx.h"
#include "type.h"
#include "xbee.h"

enum estado estadoActual = Waiting_Delimiter;
enum tipoTrama tipoTramaRecibida;
volatile uint8_t xbeeFrame[60];
volatile uint16_t currentIndex = 0;
volatile uint16_t frameDataLength = 0;
volatile uint16_t dataArrived= 0;

void CargarByte(volatile uint8_t byteReceived)
{
	switch (estadoActual)
	{
		case Waiting_Delimiter:
			if (byteReceived == 0x7E)
			{
				xbeeFrame[currentIndex++] = byteReceived;
				frameDataLength |= byteReceived << 8;
				estadoActual = Waiting_MSB;
			}
			break;

		case Waiting_MSB:
			xbeeFrame[currentIndex++] = byteReceived;
			estadoActual = Waiting_LSB;
			break;

		case Waiting_LSB:
			xbeeFrame[currentIndex++] = byteReceived;
			estadoActual = Waiting_Data;
			frameDataLength = byteReceived;
			break;

		case Waiting_Data:
			xbeeFrame[currentIndex++] = byteReceived;
			dataArrived += 1;
			if (frameDataLength == dataArrived)
				estadoActual = Waiting_Checksum;
			break;

		case Waiting_Checksum:
			xbeeFrame[currentIndex++] = byteReceived;
			estadoActual = Ready;
			break;
	}

}

void BorrarDatosFrame()
{
	currentIndex = 0;
	frameDataLength = 0;
	dataArrived = 0;
	estadoActual = Waiting_Delimiter;
}


enum tipoTrama ChequearTrama()
{

	 switch (xbeeFrame[3])
	 {
		//Receive Packet
		case 0x81:
			break;

		//AT Command Response
		case 0x88:
			return ATResponse;

		//Transmit Status
		case 0x89:
			return TransmitStatus;

		//Remote AT Command Response
		case 0x97:
			return RemoteATResponse;

		default:
			return NonValid;
	 }

	 switch (xbeeFrame[8])
	 {
		//Receive Start Packet
		case 0x10:
			return Start;

		//Receive Stop Packet
		case 0x90:
			return Stop;

		//Receive Data Packet (Mediciones de Gotas)
		case 0x12:
			return GotaDetectada;

		//Receive Data Packet (Volumen de Agua)
		case 0x13:
			return ActualizarVolumen;

		//Receive History Request
		case 0x14:
			return TransmitHistoryRequest;

		//Receive Update Transmition Time
		case 0x15:
			return UpdateRefreshTime;

		//Reset
		case 0x16:
			return Reset;

		//Receive History Response
		case 0x94:
			return TransmitHistoryResponse;

		default:
			return NonValid;
	 }
}


