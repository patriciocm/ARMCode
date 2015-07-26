/*
 * taskKeepAlibe.c
 *
 *  Created on: Sep 24, 2012
 *      Author: "Facundo Nahuel Uriel Silva"
 */

#include "taskKeepAlive.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void taskKeepAlive(void * blinkPeriode){

	const portTickType periode = ((uint32_t)blinkPeriode / 2) / portTICK_RATE_MS;
	BOOL_8 state = TRUE;

	// Se configura el pin del led del LCPXpresso como salida
	LPC_GPIO0->FIODIR |= (1<<22);

	while(1){

		if(state == TRUE){
			LPC_GPIO0->FIOCLR = (1<<22);
			state = FALSE;
		}else{
			LPC_GPIO0->FIOSET = (1<<22);
			state = TRUE;
		}

		// Se demora medio periodo
		vTaskDelay( periode );
		taskYIELD();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

