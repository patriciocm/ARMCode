/*
 * auxiliares.c
 *
 *  Created on: Nov 4, 2013
 *      Author: CJARDIN
 */
#include "auxiliares.h"
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"

volatile float volumenPrecipitacionCriterioMaximoLineal;
volatile float volumenPrecipitacionCriterioMaximoExponencial;
volatile float intensidadaPrecipitacionCriterioMaximoLineal;
volatile float intensidadaPrecipitacionCriterioMaximoExponencial;
volatile float volumenPrecipitacionCriterioDuracionLineal;
volatile float volumenPrecipitacionCriterioDuracionExponencial;
volatile float intensidadaPrecipitacionCriterioDuracionLineal;
volatile float intensidadaPrecipitacionCriterioDuracionExponencial;
volatile float volumenPrecipitacionCriterioModuloLineal;
volatile float volumenPrecipitacionCriterioModuloExponencial;
volatile float intensidadaPrecipitacionCriterioModuloLineal;
volatile float intensidadaPrecipitacionCriterioModuloExponencial;
volatile int32_t upateTime = 10;  //TiempodeUpdateDatos
volatile float superficieSensor = 3.141592*25*25;      //en mm2
volatile float transmitirXBEE = 0;      //en mm2
volatile float transmitirRS485 = 1;

void resetMediciones()
{
	volumenPrecipitacionCriterioMaximoLineal = 0;
	volumenPrecipitacionCriterioMaximoExponencial = 0;
	intensidadaPrecipitacionCriterioMaximoLineal = 0;
	intensidadaPrecipitacionCriterioMaximoExponencial = 0;
	volumenPrecipitacionCriterioDuracionLineal = 0;
	volumenPrecipitacionCriterioDuracionExponencial = 0;
	intensidadaPrecipitacionCriterioDuracionLineal = 0;
	intensidadaPrecipitacionCriterioDuracionExponencial = 0;
	volumenPrecipitacionCriterioModuloLineal = 0;
	volumenPrecipitacionCriterioModuloExponencial = 0;
	intensidadaPrecipitacionCriterioModuloLineal = 0;
	intensidadaPrecipitacionCriterioModuloExponencial = 0;
}

void enviarDatos ()
{
	volatile uint8_t tramaATransmitir[200];

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
	float constanteNormalizacion = 1000/superficieSensor;     //en 1/mm2

	char text_array[40];
	sprintf(text_array,"%.2f#%.2f#%.2f#%.2f#%.2f#%.2f", volumenPrecipitacionCriterioMaximoLineal*constanteNormalizacion , volumenPrecipitacionCriterioMaximoExponencial*constanteNormalizacion, volumenPrecipitacionCriterioModuloLineal*constanteNormalizacion, volumenPrecipitacionCriterioModuloExponencial*constanteNormalizacion, volumenPrecipitacionCriterioDuracionLineal*constanteNormalizacion, volumenPrecipitacionCriterioDuracionExponencial*constanteNormalizacion);
	int i;
	for(i=0; text_array[i]!='\0'; i++)
	{
		tramaATransmitir[9+i] = text_array[i];
		checksumAux += text_array[i];
	}

	tramaATransmitir[2]= 6 + i;
	tramaATransmitir[3 + tramaATransmitir[2]]= 0xFF - checksumAux;
	UARTSend(3, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
}

void calcularVolumen (float maximo, uint32_t sumaModulo, uint32_t duracionGota)
{
	 float volumenGota;

	 //CRITERIO MAXIMO
	 float maximoSlope = 34.788;
	 float maximoOrdenada = 0.4085;
	 float maximoMultiplicador = 0.3237;
	 float maximoExponente = 31.733;

	 //Criterio Maximo Lineal
	 volumenGota = (maximo - maximoOrdenada) / maximoSlope;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioMaximoLineal += volumenGota;

	 //Criterio Maximo Exponencial
	 volumenGota = logf(maximo/maximoMultiplicador)/maximoExponente;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioMaximoExponencial += volumenGota;

	 //CRITERIO MODULO
	 float moduloSlope = 14959603.29;
	 float moduloOrdenada = 149872.96;
	 float moduloMultiplicador = 88186;
	 float moduloExponente = 40.173;

	 //Criterio Duracion Lineal
	 volumenGota = (sumaModulo - moduloOrdenada) / moduloSlope;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioModuloLineal += volumenGota;

	 //Criterio Duracion Exponencial
	 volumenGota = logf(sumaModulo/moduloMultiplicador)/moduloExponente;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioModuloExponencial += volumenGota;

	 //CRITERIO DURACION
	 float duracionSlope = 28234;
	 float duracionOrdenada = 1883.4;
	 float duracionMultiplicador = 1651;
	 float duracionExponente = 12.878;

	 //Criterio Duracion Lineal
	 volumenGota = (duracionGota - duracionOrdenada) / duracionSlope;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioDuracionLineal += volumenGota;

	 //Criterio Duracion Exponencial
	 volumenGota = logf(duracionGota/duracionMultiplicador)/duracionExponente;
	 if ( volumenGota < 0)
	 {	 volumenGota = 0;
	 }
	 volumenPrecipitacionCriterioDuracionExponencial += volumenGota;
}


