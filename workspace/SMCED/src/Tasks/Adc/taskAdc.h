/*
 * taskAdc.h
 *
 *  Created on: Oct 27, 2012
 *      Author: "Facundo Nahuel Uriel Silva"
 */

#ifndef TASKADC_H_
#define TASKADC_H_

#include "LPC17xx.h"

#include "lpc17xx_pinsel.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_rtc.h"

#include <math.h>
//#include "Tasks/SdCard/taskSdCard.h"

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"

#include "../Tasks/SdCard/ff.h"

extern FATFS fs;

#define LAST_SAMPLES_FILE "samples.csv"
#define MEDICION_FILE "medicion.csv"
#define HEADER_CSV_FILE "Tension[V];Corriente[A];Factor Potencia;frecuencia[Hz];Energia[W/h];\n"

typedef enum{
	ADC_0,
	ADC_1,
	ADC_2,
	ADC_3,
	ADC_4
} ADC_type;

typedef struct{
	int32_t Adc0Value;
	int32_t Adc1Value;
} SensorsValues;

typedef enum{
	ADC_SINCRONIZE,
	ADC_SINCRONIZING_DESC_START,
	ADC_SINCRONIZING_DESC_WAIT_FIRST,
	ADC_SINCRONIZING_DESC_WAIT_SECOND,
	ADC_SINCRONIZING_ASC_WAIT_FIRST,
	ADC_SINCRONIZING_ASC_WAIT_SECOND,
	ADC_START,
	ADC_GET_ADC0_SAMPLE,
	ADC_GET_ADC1_SAMPLE,
	ADC_GIVE_MUTEX
} ADC_states;

#define PI 3.14159265
#define PI2 6.28318543

#define MUESTRAS_POR_PERIODO  50
#define CANTIDAD_PERIODOS  10
#define SIZE_SAMPLES MUESTRAS_POR_PERIODO*CANTIDAD_PERIODOS
#define MUESTRAS_CHECK_SINCRONISMO  MUESTRAS_POR_PERIODO/4
#define CICLOS_SEGUNDO MUESTRAS_POR_PERIODO/CANTIDAD_PERIODOS
#define NUMERO_SEGUNDOS_GUARDAR_VALORES 30
#define CANTIDAD_CICLOS  CICLOS_SEGUNDO*NUMERO_SEGUNDOS_GUARDAR_VALORES

#define UMBRAL_CERO 480
#define UMBRAL_TECHO 1000
#define UMBRAL_FLACO 100
#define UMBRAL_RESINCRONISMO UMBRAL_CERO

#define OFFSET_VALOR_TENSION 500
#define OFFSET_VALOR_CORRIENTE 2176

#define FACTOR_CONVERSION_TENSION (float)(220.0/1760)
#define FACTOR_CONVERSION_CORRIENTE (float)(0.45/251)
#define FACTOR_PHI (float)(PI2/MUESTRAS_POR_PERIODO)
#define FACTOR_FRECUENCIA 4
#define FACTOR_KILOWATT_HORA (float)(NUMERO_SEGUNDOS_GUARDAR_VALORES/(2.0*3600)) // 1 W/h = 3600 J


typedef struct{
	SensorsValues values[SIZE_SAMPLES];
} medicion;

/** \brief
 *
 * @param param
 */
void taskAdc(void *param);

int16_t getAdcValue(ADC_type adc);

void synchronize();

void synchronizeNextSampling();

void ADC_IRQHandler(void);

#endif /* TASKADC_H_ */
