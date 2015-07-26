/*
 * auxiliares.c
 *
 *  Created on: Nov 4, 2013
 *      Author: CJARDIN
 */
#include "auxiliares.h"

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





