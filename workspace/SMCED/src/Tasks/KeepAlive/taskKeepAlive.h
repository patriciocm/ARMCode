/*
 * taskKeepAlibe.h
 *
 *  Created on: Sep 24, 2012
 *      Author: "Facundo Nahuel Uriel Silva"
 */

#ifndef TASKKEEPALIBE_H_
#define TASKKEEPALIBE_H_

#include "LPC17xx.h"
#include "lpc_types.h"
#include "FreeRTOS.h"
#include "task.h"

/** \brief Tarea para implementar un keep alive con el led del LpcXpresso
 *
 * @param blinkPeriode Periodo de encendido/apagado en milisegundos
 */
void taskKeepAlive(void * blinkPeriode);

#endif /* TASKKEEPALIBE_H_ */
