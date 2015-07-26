#include "LPC17xx.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "auxiliares.h"
#include "uart.h"
#include "type.h"
#include "rtc.h"
#include "xbee.h"
#include "uart.h"

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
	 	//Transmit Request Packet
	 	case 0x01:
	 		break;

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

		//Receive Start Packet
		case 0x11:
			return StartOk;

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
			return ResetMedicionesLluvia;

		//Receive Update Transmition Time
		case 0x15:
			return UpdateRefreshTime;

		//Receive History Request
		case 0x16:
			return TransmitHistoryGotaDetectada;

		//Receive History Request
		case 0x17:
			return TransmitHistoryVolumenIntesidad;

		//Receive History Request
		case 0x20:
			return RTCReadRequest;

		//Receive History Request
		case 0x22:
			return RTCSet;

		default:
			return NonValid;
	 }
}

void EnviarStartOk(int8_t UART)
{
	uint8_t tramaATransmitir[10];

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x06;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;
	tramaATransmitir[8] = 0x11;
	tramaATransmitir[9] = 0xEB;

	UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
}

void EnviarFinishHistoryTransmission(int8_t UART)
{
	uint8_t tramaATransmitir[10];

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x06;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;
	tramaATransmitir[8] = 0x18;
	tramaATransmitir[9] = 0xE4;

	UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
}

void EnviarStopOk(int8_t UART)
{
	uint8_t tramaATransmitir[10];

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x06;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;
	tramaATransmitir[8] = 0x91;
	tramaATransmitir[9] = 0x6B;

	UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
}

void EnviarRTCReadResponse(int8_t UART)
{
	uint8_t tramaATransmitir[50];
	RTCTime horaActual;

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x06;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;
	tramaATransmitir[8] = 0x21;

	uint8_t checksumAux = 0x24;

	horaActual = RTCGetTime();

	sprintf(&tramaATransmitir[9],"%.2d#%.2d#%.4d#%.2d#%.2d#%.2d", horaActual.RTC_Mday, horaActual.RTC_Mon, horaActual.RTC_Year, horaActual.RTC_Hour, horaActual.RTC_Min, horaActual.RTC_Sec);
	int i;
	for(i=9; tramaATransmitir[i]!='\0'; i++)
	{
		checksumAux += tramaATransmitir[i];
	}

	tramaATransmitir[2]= 6 + (i-9);
	tramaATransmitir[3 + tramaATransmitir[2]]= 0xFF - checksumAux;

	UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );

}

void SetearRTC(int8_t dia, int8_t mes, int16_t anio, int8_t hora, int8_t minuto, int8_t segundo)
{
	RTCTime tiempo;

	tiempo.RTC_Sec = segundo;
	tiempo.RTC_Min = minuto;
	tiempo.RTC_Hour = hora;
	tiempo.RTC_Mday = dia;
	tiempo.RTC_Mon = mes;
	tiempo.RTC_Year = anio;

	RTCSetTime( tiempo );

}



