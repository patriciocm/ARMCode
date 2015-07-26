/****************************************************************************
 *   $Id:: rtc.c 5743 2010-11-30 23:18:58Z usb00423                         $
 *   Project: NXP LPC17xx RTC example
 *
 *   Description:
 *     This file contains RTC code example which include RTC initialization, 
 *     RTC interrupt handler, and APIs for RTC access.
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
#include "rtc.h"
#include "auxiliares.h"


volatile uint32_t alarm_on_XBEE = 0, alarm_on_RS485 = 0, alarm_on_SD = 0, countRTC_XBEE = 0, countRTC_RS485 = 0, countRTC_SD = 0;
volatile RTCTime local_time, alarm_time, current_time;

/*****************************************************************************
** Function name:		RTC_IRQHandler
**
** Descriptions:		RTC interrupt handler, it executes based on the
**						the alarm setting
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_IRQHandler (void)
{
    LPC_RTC->ILR |= ILR_RTCCIF;		/* clear interrupt flag */

    elapsedTimeSinceLastVolumeUpdate++;

    if(saveSD)
    {	countRTC_SD++;
		if(countRTC_SD >= updateTimeSD)
		{
			alarm_on_SD = TRUE;
			countRTC_SD = 0;
		}
		return;
    }

    if(transmitirXBEE)
    {	countRTC_XBEE++;
    	if(countRTC_XBEE >= updateTimeXBEE)
        {
        	alarm_on_XBEE = TRUE;
        	countRTC_XBEE = 0;
        }
    }

    if(transmitirRS485)
    {	countRTC_RS485++;
    	if(countRTC_RS485 >= updateTimeRS485)
    	{
    		alarm_on_RS485 = TRUE;
    		countRTC_RS485 = 0;
    	}
    }
}

/*****************************************************************************
** Function name:		RTCInit
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCInit( void )
{

  /* Enable CLOCK into RTC */
  LPC_SC->PCONP |= (1 << 9);

  /* If RTC is stopped, clear STOP bit. */
  if ( LPC_RTC->RTC_AUX & (0x1<<4) )
  {
	LPC_RTC->RTC_AUX |= (0x1<<4);	
  }
  
  /*--- Initialize registers ---*/    
  LPC_RTC->AMR = 0;
  LPC_RTC->CIIR = 0;
  LPC_RTC->CCR = 0;

  local_time.RTC_Sec = 0;
  local_time.RTC_Min = 56;
  local_time.RTC_Hour = 20;
  local_time.RTC_Mday = 6;
  local_time.RTC_Wday = 0;
  local_time.RTC_Yday = 0;		/* current date 14/07/2013 */
  local_time.RTC_Mon = 5;
  local_time.RTC_Year = 2014;
  //RTCSetTime( local_time );		/* Set local time */

  NVIC_EnableIRQ(RTC_IRQn);

  LPC_RTC->CIIR = IMSEC;    //Generar Interrupcion cada 1seg
  RTCSetAlarmMask(AMRSEC|AMRMIN|AMRHOUR|AMRDOM|AMRDOW|AMRDOY|AMRMON|AMRYEAR);

  return;
}

/*****************************************************************************
** Function name:		RTCStart
**
** Descriptions:		Start RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStart( void ) 
{
  /*--- Start RTC counters ---*/
  LPC_RTC->CCR |= CCR_CLKEN;
  LPC_RTC->ILR = ILR_RTCCIF;
  return;
}

/*****************************************************************************
** Function name:		RTCStop
**
** Descriptions:		Stop RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStop( void )
{   
  /*--- Stop RTC counters ---*/
  LPC_RTC->CCR &= ~CCR_CLKEN;
  return;
} 

/*****************************************************************************
** Function name:		RTC_CTCReset
**
** Descriptions:		Reset RTC clock tick counter
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTC_CTCReset( void )
{   
  /*--- Reset CTC ---*/
  LPC_RTC->CCR |= CCR_CTCRST;
  return;
}

/*****************************************************************************
** Function name:		RTCSetTime
**
** Descriptions:		Setup RTC timer value
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetTime( RTCTime Time ) 
{
  LPC_RTC->SEC = Time.RTC_Sec;
  LPC_RTC->MIN = Time.RTC_Min;
  LPC_RTC->HOUR = Time.RTC_Hour;
  LPC_RTC->DOM = Time.RTC_Mday;
  LPC_RTC->DOW = Time.RTC_Wday;
  LPC_RTC->DOY = Time.RTC_Yday;
  LPC_RTC->MONTH = Time.RTC_Mon;
  LPC_RTC->YEAR = Time.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCSetAlarm
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarm( RTCTime Alarm ) 
{   
  LPC_RTC->ALSEC = Alarm.RTC_Sec;
  LPC_RTC->ALMIN = Alarm.RTC_Min;
  LPC_RTC->ALHOUR = Alarm.RTC_Hour;
  LPC_RTC->ALDOM = Alarm.RTC_Mday;
  LPC_RTC->ALDOW = Alarm.RTC_Wday;
  LPC_RTC->ALDOY = Alarm.RTC_Yday;
  LPC_RTC->ALMON = Alarm.RTC_Mon;
  LPC_RTC->ALYEAR = Alarm.RTC_Year;    
  return;
}

/*****************************************************************************
** Function name:		RTCGetTime
**
** Descriptions:		Get RTC timer value
**
** parameters:			None
** Returned value:		The data structure of the RTC time table
** 
*****************************************************************************/
RTCTime RTCGetTime( void ) 
{
  RTCTime LocalTime;
    
  LocalTime.RTC_Sec = LPC_RTC->SEC;
  LocalTime.RTC_Min = LPC_RTC->MIN;
  LocalTime.RTC_Hour = LPC_RTC->HOUR;
  LocalTime.RTC_Mday = LPC_RTC->DOM;
  LocalTime.RTC_Wday = LPC_RTC->DOW;
  LocalTime.RTC_Yday = LPC_RTC->DOY;
  LocalTime.RTC_Mon = LPC_RTC->MONTH;
  LocalTime.RTC_Year = LPC_RTC->YEAR;
  return ( LocalTime );    
}

/*****************************************************************************
** Function name:		RTCSetAlarmMask
**
** Descriptions:		Set RTC timer alarm mask
**
** parameters:			Alarm mask setting
** Returned value:		None
** 
*****************************************************************************/
void RTCSetAlarmMask( uint32_t AlarmMask ) 
{
  /*--- Set alarm mask ---*/    
  LPC_RTC->AMR = AlarmMask;
  return;
}

void RTCPrintTime (RTCTime Time )
{

	debug_printf("Fecha: %d/%d/%d\n", Time.RTC_Mday, Time.RTC_Mon, Time.RTC_Year);
	debug_printf("Hora: %d:%d:%d\n", Time.RTC_Hour, Time.RTC_Min, Time.RTC_Sec);

}

/*****************************************************************************
**                            End Of File
******************************************************************************/

