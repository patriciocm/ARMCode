#ifndef __AUX_H
#define __AUX_H

#include <stdint.h>

extern volatile float volumenPrecipitacionCriterioMaximoLineal;
extern volatile float volumenPrecipitacionCriterioMaximoExponencial;
extern volatile float intensidadaPrecipitacionCriterioMaximoLineal;
extern volatile float intensidadaPrecipitacionCriterioMaximoExponencial;
extern volatile float volumenPrecipitacionCriterioDuracionLineal;
extern volatile float volumenPrecipitacionCriterioDuracionExponencial;
extern volatile float intensidadaPrecipitacionCriterioDuracionLineal;
extern volatile float intensidadaPrecipitacionCriterioDuracionExponencial;
extern volatile float volumenPrecipitacionCriterioModuloLineal;
extern volatile float volumenPrecipitacionCriterioModuloExponencial;
extern volatile float intensidadaPrecipitacionCriterioModuloLineal;
extern volatile float intensidadaPrecipitacionCriterioModuloExponencial;
extern volatile int32_t upateTime;  //Tiempo de Update de Datos en segundos
extern volatile float superficieSensor;      //en mm2
extern volatile float transmitirXBEE;      //en mm2
extern volatile float transmitirRS485;      //en mm2

extern void resetMediciones();
extern void calcularVolumen (float maximo, uint32_t sumaModulo, uint32_t duracionGota);
extern void enviarDatos ();

#endif
