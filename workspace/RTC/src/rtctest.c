/****************************************************************************
 *   $Id:: rtctest.c 6097 2011-01-07 04:31:25Z nxp12832                     $
 *   Project: NXP LPC17xx RTC example
 *
 *   Description:
 *     This file contains RTC test modules, main entry, to test RTC APIs.
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
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "type.h"
#include "rtc.h"
#include "debug.h"
#include "uart.h"
#include "auxiliares.h"

extern volatile uint32_t alarm_on;
RTCTime local_time, alarm_time, current_time;

/*****************************************************************************
**   Main Function  main()
******************************************************************************/
int main (void)
{ 
  /* SystemClockUpdate() updates the SystemFrequency variable */
  SystemCoreClockUpdate();
  debug_printf("CPU Clock:%dMHz\n", SystemCoreClock/1000000);

  /* Initialize RTC module */
  RTCInit();

  local_time.RTC_Sec = 0;
  local_time.RTC_Min = 15;
  local_time.RTC_Hour = 16;
  local_time.RTC_Mday = 7;
  local_time.RTC_Wday = 5;
  local_time.RTC_Yday = 0;		/* current date 07/12/2006 */
  local_time.RTC_Mon = 12;
  local_time.RTC_Year = 2013;
  //RTCSetTime( local_time );		/* Set local time */

  alarm_time.RTC_Sec = 10;
  alarm_time.RTC_Min = 12;
  alarm_time.RTC_Hour = 0;
  alarm_time.RTC_Mday = 0;
  alarm_time.RTC_Wday = 0;
  alarm_time.RTC_Yday = 1;		/* alarm date 01/01/2007 */
  alarm_time.RTC_Mon = 1;
  alarm_time.RTC_Year = 2007;

  RTCSetAlarm( alarm_time );		/* set alarm time */

  NVIC_EnableIRQ(RTC_IRQn);

  /* mask off alarm mask, turn on IMYEAR in the counter increment interrupt
  register */
  RTCSetAlarmMask(AMRMIN|AMRHOUR|AMRDOM|AMRDOW|AMRDOY|AMRMON|AMRYEAR);
    
  RTCStart();
  RTCPrintTime (RTCGetTime());

  while (1) 
  {
	  /* Loop forever */
	  if ( alarm_on != 0 )
	  {
		  alarm_on = 0;
		  /* Get current time when alarm is on */
		  RTCPrintTime (RTCGetTime());

	  }
  }

}

/******************************************************************************
**                            End Of File
******************************************************************************/
