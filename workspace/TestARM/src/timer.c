/****************************************************************************
 *   $Id:: timer.c 5823 2010-12-07 19:01:00Z usb00423                       $
 *   Project: NXP LPC17xx Timer for PWM example
 *
 *   Description:
 *     This file contains timer code example which include timer 
 *     initialization, timer interrupt handler, and APIs for timer access.
 *
 ****************************************************************************
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
****************************************************************************/
#include "LPC17xx.h"
#include "type.h"
#include "timer.h"
#include "debug.h"


volatile uint32_t timer0_m0_counter = 0;
volatile uint32_t timer0_m1_counter = 0;

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
** 
******************************************************************************/
void TIMER0_IRQHandler (void) 
{  
  if ( LPC_TIM0->IR & (0x1<<0) )
  {
    LPC_TIM0->IR = 0x1<<0;		/* clear interrupt flag */
    timer0_m0_counter++;
  }
  return;
}


/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
** 
******************************************************************************/
void enable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1 oe 2 or 3
** Returned value:		None
** 
******************************************************************************/
void disable_timer( uint8_t timer_num )
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1 or 2 or 3
** Returned value:		None
** 
******************************************************************************/
void reset_timer( uint8_t timer_num )
{
  uint32_t regVal;

  if ( timer_num == 0 )
  {
	regVal = LPC_TIM0->TCR;
	regVal |= 0x02;
	LPC_TIM0->TCR = regVal;
  }
  return;
}

/******************************************************************************
** Function name:		init_timer
**
** Descriptions:		Initialize timer, set timer interval, reset timer,
**						install timer interrupt handler
**
** parameters:			timer number and timer interval
** Returned value:		true or false, if the interrupt handler can't be
**						installed, return false.
** 
******************************************************************************/
uint32_t init_timer ( uint8_t timer_num)
{
  uint32_t pclkdiv, pclk;

  if ( timer_num == 0 )
  {
	timer0_m0_counter = 0;

	LPC_SC->PCONP |= (0x01<<1);

	LPC_TIM0->IR = 0x01;          /* Clear MATx interrupt include DMA request */

	/* By default, the PCLKSELx value is zero, thus, the PCLK for
	all the peripherals is 1/4 of the SystemFrequency. */
	/* Bit 2~3 is for TIMER0 */
	pclkdiv = (LPC_SC->PCLKSEL0 >> 2) & 0x03;
	switch ( pclkdiv )
	{
	  case 0x00:
		pclk = SystemCoreClock/4;
		break;
	  case 0x01:
		pclk = SystemCoreClock;
		break; 
	  case 0x02:
		pclk = SystemCoreClock/2;
		break; 
	  case 0x03:
		pclk = SystemCoreClock/8;
		break;
	}

	LPC_TIM0->PR  = pclk/1000000; /* set prescaler  */

/*#ifdef DEBUG
    		debug_printf("Valor del PreScaler:%d \n", LPC_TIM0->PR);
    		debug_printf_flush();
	#endif*/

	LPC_TIM0->MR0 = 10; /* Set up 10 us interval */
	LPC_TIM0->MCR = (0x3<<0);	/* Interrupt and Reset on MR0 and MR1 */
	NVIC_EnableIRQ(TIMER0_IRQn);
	return (TRUE);
  }

  return (FALSE);
}

/******************************************************************************
**                            End Of File
******************************************************************************/
