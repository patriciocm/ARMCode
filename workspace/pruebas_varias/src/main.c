/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : Copyright (C) 
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <math.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// TODO: insert other include files here
#include "debug.h"
#include "rittimer.h"

// TODO: insert other definitions and declarations here
volatile uint32_t ritCount = 0;
extern volatile uint32_t rit_timer_counter;

/*
void RIT_IRQHandler(void)
{
	LPC_RIT->RICTRL |= 0x01 << 0;
	ritCount++;
	return;

}

void InitRIT(void)
{
	LPC_SC->PCONP |= 1 << 16;

	LPC_RIT->RICTRL |=0xf7;
	LPC_RIT->RICOMPVAL = 500;				//Valor hasta el que se cuenta antes de que se genere interrupcion
	LPC_RIT->RIMASK = 0xffffffff;
	LPC_RIT->RICTRL |= 0x03;			//Borra contador en la interrupcion
	//LPC_RIT->RICTRL &= ~( 0x01 << 2);     //Debug no afecta operacion de timer
	LPC_SC->PCLKSEL1 |= (1 << 26); 			//Setea bit 26 en 1
	LPC_SC->PCLKSEL1 &= ~( 0x01 << 27); 	//Setea bit 27 en 0
	NVIC_EnableIRQ(RIT_IRQn);
	return;
}
*/

int main(void)
{
	float SUPPLY_VOLTAGE = 3.3;    //en volts
	double ADC_COUNT_MAX = 4095;
	float ADC_STEP = SUPPLY_VOLTAGE/ADC_COUNT_MAX;

	//Update variable SystemCoreClock
	SystemCoreClockUpdate();
	#ifdef DEBUG
    	debug_printf("CPU Clock:%dMHz\n", SystemCoreClock/1000000);
    	debug_printf_flush();
	#endif

	// Enter an infinite loop, just incrementing a counter
	uint32_t count = 0;

	//Incializar RIT Timer
	init_rit_timer( TIME_INTERVAL );		/* 10ms */
	enable_rit_timer();

	int32_t variableA = 100;
	float variableB;
	float variableC;
	float variableD;
	float aux;
	int16_t i;

	while(1)
	{
		/*
		rit_timer_counter = 0;
		while( rit_timer_counter < 100000 )
		{
		}
		count++;
		*/

		rit_timer_counter = 0;
		for(i=0; i < 30000; i++)
		{
			aux = variableA*ADC_STEP;
			//variableB+=aux;
			//variableB += variableA*ADC_STEP;
			//variableC = aux*aux;
			//variableD += variableC;
		}
		int32_t tiempoFinal = rit_timer_counter;

		//#ifdef DEBUG
			//debug_printf("El valor de tiempoInical es: %d\n", tiempoInicial);
			debug_printf("El valor de tiempoFinal es: %d\n", tiempoFinal);
			//debug_printf("El valor de %d al cuadrado es: %d\n", variableA, variableB);
			//debug_printf("Interrupcion de Timer:%d \n", count);
			debug_printf_flush();
		//#endif
    }
	return 0;
}













