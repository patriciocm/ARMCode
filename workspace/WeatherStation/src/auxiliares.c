/*
 * auxiliares.c
 *
 *  Created on: Nov 4, 2013
 *      Author: CJARDIN
 */
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "auxiliares.h"
#include "uart.h"
#include "type.h"
#include "rtc.h"
#include "ff.h"
#include "timer.h"
#include "xbee.h"


volatile float volumenPrecipitacion;
volatile float intensidadaPrecipitacion;
volatile float alfa = 0.0125;
volatile float beta = 0.4;

volatile int32_t updateTimeXBEE = 10;  //TiempodeUpdateDatos
volatile int32_t updateTimeRS485 = 10;  //TiempodeUpdateDatos
volatile int32_t updateTimeSD = 15;

volatile int8_t transmitirXBEE = FALSE;
volatile int8_t transmitirRS485 = FALSE;
volatile int8_t saveSD = TRUE;
volatile int8_t startReceived = FALSE;
volatile int8_t startReceivedUART;
volatile int8_t stopReceived = FALSE;
volatile int8_t stopReceivedUART;
volatile int8_t rtcReadRequestReceived = FALSE;
volatile int8_t rtcReadRequestReceivedUART;
volatile float lastSendVolume = 0;
volatile int32_t elapsedTimeSinceLastVolumeUpdate = 0;

volatile FIL * ptrFileBackup;

extern volatile uint32_t timer0_m0_counter;


void ResetMediciones()
{
	volumenPrecipitacion = 0;
	lastSendVolume = 0;
}

void CalcularVolumen (float maximo)
{
	 //CRITERIO MAXIMO
	 float volumenGotaNormalizado;
	 //uint32_t tiempoInicial, tiempoFinal, duracionSenial;

	 //Criterio Maximo Lineal
	 //tiempoInicial = timer0_m0_counter;
	 volumenGotaNormalizado = alfa*powf(maximo,beta);
	 //duracionSenial = timer0_m0_counter - tiempoInicial;

	 volumenPrecipitacion += volumenGotaNormalizado;
}


//MODO AUTONOMO: GRABAR EN MEMORIA MICRO SD
void SaveDropDataSD(FIL *fp, float tensionGota)
{
	char Buff[8*16];
	RTCTime horaActual;
	uint16_t bytesWritten;
	//uint32_t tiempoInicial, tiempoFinal, duracionSenial;

	horaActual = RTCGetTime();

	sprintf(Buff,"%.2d#%.2d#%.4d#%.2d#%.2d#%.2d#%5.2f\n", horaActual.RTC_Mday, horaActual.RTC_Mon, horaActual.RTC_Year, horaActual.RTC_Hour, horaActual.RTC_Min, horaActual.RTC_Sec, tensionGota);

	//tiempoInicial = timer0_m0_counter;
	if(fp->fsize)
		f_lseek(fp, fp->fsize);
	f_write(fp, Buff, strlen(Buff), &bytesWritten);
	f_sync(fp);
	//duracionSenial = timer0_m0_counter - tiempoInicial;

	return;
}

void UpdatePrecipitationDataSD(FIL *fp)
{
	char Buff[8*16];
	RTCTime horaActual;
	uint16_t bytesWritten;
	//uint32_t tiempoInicial, tiempoFinal, duracionSenial;

	horaActual = RTCGetTime();

	intensidadaPrecipitacion = 3600*(volumenPrecipitacion - lastSendVolume) / elapsedTimeSinceLastVolumeUpdate;

	sprintf(Buff,"%.2d#%.2d#%.4d#%.2d#%.2d#%.2d#%7.2f#%7.2f\n", horaActual.RTC_Mday, horaActual.RTC_Mon, horaActual.RTC_Year, horaActual.RTC_Hour, horaActual.RTC_Min, horaActual.RTC_Sec, volumenPrecipitacion, intensidadaPrecipitacion);

	//tiempoInicial = timer0_m0_counter;
	if(fp->fsize)
		f_lseek(fp, fp->fsize);
	f_write(fp, Buff, strlen(Buff), &bytesWritten);
	f_sync(fp);
	//duracionSenial = timer0_m0_counter - tiempoInicial;

	lastSendVolume = volumenPrecipitacion;
	elapsedTimeSinceLastVolumeUpdate = 0;

	return;
}



//CONEXION REMOTA CON PC
void UpdatePrecipitationDataUserInterface (int8_t UART) //Si interfaz es 1 la transmision es por XBEE, si es 2 la transmision es por RS485
{
	uint8_t tramaATransmitir[50];
	uint16_t bytesWritten;
	RTCTime horaActual;

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x07;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;
	tramaATransmitir[8] = 0x13;

	uint8_t checksumAux = 0x16;

	ptrFileBackup = &fileBackupPrecipitacion;

	horaActual = RTCGetTime();

	intensidadaPrecipitacion = 3600*(volumenPrecipitacion - lastSendVolume) / elapsedTimeSinceLastVolumeUpdate;

	sprintf(&tramaATransmitir[9],"%.2d#%.2d#%.4d#%.2d#%.2d#%.2d#%.2f#%.2f", horaActual.RTC_Mday, horaActual.RTC_Mon, horaActual.RTC_Year, horaActual.RTC_Hour, horaActual.RTC_Min, horaActual.RTC_Sec, volumenPrecipitacion, intensidadaPrecipitacion);
	int i;
	for(i=9; tramaATransmitir[i]!='\0'; i++)
	{
		checksumAux += tramaATransmitir[i];
	}

	tramaATransmitir[2]= 6 + (i-9);
	tramaATransmitir[3 + tramaATransmitir[2]]= 0xFF - checksumAux;

	UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );

	//GUARDAR EN ARCHIVO DE BACKUP
	tramaATransmitir[i]= '\n';
	if(ptrFileBackup->fsize)
			f_lseek(ptrFileBackup, ptrFileBackup->fsize);
		f_write(ptrFileBackup, &tramaATransmitir[9], (i-9+1), &bytesWritten);
	f_sync(ptrFileBackup);

	lastSendVolume = volumenPrecipitacion;
	elapsedTimeSinceLastVolumeUpdate = 0;
}

void TransferirDatosSDtoUserInterface(FIL *fp, int8_t UART, enum tipoTrama tramaType)
{
	uint8_t tramaATransmitir[50];
	char nombreArchivo[70];
	RTCTime horaActual;
	uint16_t bytesRead, bytesWritten;

	tramaATransmitir[0] = 0x7E;
	tramaATransmitir[1] = 0x00;
	tramaATransmitir[2] = 0x07;
	tramaATransmitir[3] = 0x01;
	tramaATransmitir[4] = 0x01;
	tramaATransmitir[5] = 0x00;
	tramaATransmitir[6] = 0x01;
	tramaATransmitir[7] = 0x00;

	horaActual = RTCGetTime();
	uint8_t checksumAux, tamanioLinea;

	if(tramaType == TransmitHistoryGotaDetectada)
	{
		tramaATransmitir[8] = 0x16;
		checksumAux = 0x19;
		tamanioLinea = 26;

		//CREAR ARCHIVO DE TEXTO DONDE SE GUARDARAN LOS DATOS
		/*
		sprintf(nombreArchivo,"distribucionGotas.backup\n", horaActual.RTC_Year, horaActual.RTC_Mon, horaActual.RTC_Mday);
		f_open(&fileBackup, nombreArchivo, 0x13);*/
		ptrFileBackup = &fileBackupGotas;
	}
	else
	{
		tramaATransmitir[8] = 0x17;
		checksumAux = 0x1A;
		tamanioLinea = 36;
		ptrFileBackup = &fileBackupPrecipitacion;
	}

	uint8_t checksum = checksumAux;
	uint8_t i;
	uint32_t j=0;
	BYTE result;

	f_lseek(fp, 0);
	uint32_t tamanio = fp->fsize;


	while(tamanio >= tamanioLinea)
	{
		result = f_read(fp, &tramaATransmitir[9], tamanioLinea, &bytesRead);
		//result = LeerArchivo(&fp, &tramaATransmitir[9], tamanioLinea);

		if(result == FR_OK)
		{
			tramaATransmitir[0] = 0x7E;
			j++;
			for(i=9; tramaATransmitir[i]!='\n'; i++)
			{
				checksum += tramaATransmitir[i];
			}

			tramaATransmitir[2]= 6 + (i-9);
			tramaATransmitir[3 + tramaATransmitir[2]]= 0xFF - checksum;
			UARTSend(UART, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
			tamanio-=tamanioLinea;
			f_lseek(fp, tamanioLinea*j);
			checksum = checksumAux;
			//Delay 10ms
			uint32_t tiempoInicial = timer0_m0_counter;
			while(timer0_m0_counter < tiempoInicial + 500)
			{
			}

			//GUARDAR COMO BACKUP EN ARCHIVO DE TEXTO
			if(ptrFileBackup->fsize)
				f_lseek(ptrFileBackup, ptrFileBackup->fsize);
			tramaATransmitir[9+bytesRead-1] = '\n';
			f_write(ptrFileBackup, &tramaATransmitir[9], bytesRead, &bytesWritten);
			f_sync(ptrFileBackup);

		}
		else
		{	return;
		}
	}
}



















