/***********************************************************************
 * $Id::                                                               $
 *
 * Project:	uart: Simple UART echo for LPCXpresso 1700
 * File:	uarttest.c
 * Description:
 * 			LPCXpresso Baseboard uses pins mapped to UART3 for
 * 			its USB-to-UART bridge. This application simply echos
 * 			all characters received.
 *
 ***********************************************************************
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * products. This software is supplied "AS IS" without any warranties.
 * NXP Semiconductors assumes no responsibility or liability for the
 * use of the software, conveys no license or title under any patent,
 * copyright, or mask work right to the product. NXP Semiconductors
 * reserves the right to make changes in the software without
 * notification. NXP Semiconductors also make no representation or
 * warranty that such application will be suitable for the specified
 * use without further testing or modification.
 **********************************************************************/

/*****************************************************************************
 *   History
 *   2010.07.01  ver 1.01    Added support for UART3, tested on LPCXpresso 1700
 *   2009.05.27  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include "LPC17xx.h"
#include "type.h"
#include "uart.h"
#include "rittimer.h"
#include "debug.h"
#include <string.h>

extern uint32_t rit_timer_counter;
volatile uint32_t ritCount = 0;
extern volatile uint32_t UART3Count;
extern volatile uint8_t UART3Buffer[BUFSIZE];


void delayFunction(uint32_t delay)
{

	uint32_t tiempoEspera = rit_timer_counter + delay*100;
	while( rit_timer_counter < tiempoEspera )
	{}
	return;
}

/*****************************************************************************
**   Main Function  main()
This program has been test on LPCXpresso 1700.
*****************************************************************************/
int main (void)
{
	const char* welcomeMsg = "UART3 Online:\r\n";
	//SystemInit();	//Called by startup code

	UARTInit(0, 9600);	/* baud rate setting */

	uint8_t array[11];

	array[0] = 0x7E;
	array[1] = 0x00;
	array[2] = 0x07;
	array[3] = 0x01;
	array[4] = 0x01;
	array[5] = 0x00;
	array[6] = 0x01;
	array[7] = 0x00;
	array[8] = 0x12;
	array[9] = 0x00;
	array[10] = 0xEA;

	init_rit_timer( TIME_INTERVAL );		/* 10us */
	enable_rit_timer();

	uint32_t tiempoInicial = 0;
	uint32_t tiempoFinal = 0;
	uint16_t i = 0;
	uint16_t entro = 1;

	debug_printf("COMIENZO DEL PROGRAMA\n");
	/* Loop forever */
	while (1)
	{
		if(entro)
		{	tiempoInicial = rit_timer_counter;
			for(i=0; i < 5; i++)
			{
				UARTSend(0, (uint8_t *)array , 11 );
				delayFunction(4);   //Espero 4ms
				array[9] += 0x01;
				array[10] -= 0x01;

			}
			tiempoFinal = rit_timer_counter;

			debug_printf("Tiempo Total de Transmision: %d\n", (int) (tiempoFinal-tiempoInicial) );
			debug_printf_flush();
			entro = 0;
		}
		/*
		if ( UART3Count != 0 )
		{
			LPC_UART3->IER = IER_THRE | IER_RLS;				//Disable RBR
			UARTSend(3, (uint8_t *)UART3Buffer, UART3Count );
			UART3Count = 0;
			LPC_UART3->IER = IER_THRE | IER_RLS | IER_RBR;		//Re-enable RBR

		}*/
	}
}

/*****************************************************************************
**                            End Of File
*****************************************************************************/
