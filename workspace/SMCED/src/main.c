/**
 * Name        : main.c
 * Version     :
 * Description : main definition for FreeRTOS application
 */

/*
 * FreeRTOS includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "Tasks/KeepAlive/taskKeepAlive.h"
#include "Tasks/PcCommunication/taskPcCommunication.h"
#include "Tasks/Adc/taskAdc.h"
//#include "Tasks/SerialMemory/taskSerialMemory.h"
//#include "Tasks/Core/taskCore.h"
#include "Tasks/SdCard/taskSdCard.h"
#include "Tasks/WebServer/taskWebServer.h"

#include "lpc17xx_rtc.h"

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE

void __error__(char *pcFilename, unsigned long ulLine) {
}

extern xQueueHandle queueAdcSdCard;

xSemaphoreHandle semSdCardAccess;
extern xQueueHandle queueSdCardWrite;
extern xQueueHandle queueSdCardRead;
extern xSemaphoreHandle semSdCardReadAction;
extern xQueueHandle queueAdcSdCard;

FATFS fs;

static void setupHardware(void) {
	// Se configuran los perifericos por defecto
	SystemInit();

	// Se inicializa las task de comunicaciones
//	taskPcCommunicationInit();

	semSdCardAccess = xSemaphoreCreateMutex();
	xSemaphoreGive(semSdCardAccess);

	// SD Card
	LPC_GPIO0->FIODIR |= 1<<22;
	LPC_GPIO0->FIOCLR |= 1<<22;

	LPC_SC->PCLKSEL0 &= ~(3<<2);
	LPC_SC->PCLKSEL0 |= 1<<2;

	/*
	 * Default values for the SPI clock
	 * Use 400 kHz during init and 1 MHz during data transfer
	 *
	 * These values are believed to be reasonably safe values.
	 */
	SetSPIClocks(KHZ(400), MHZ(1));

	f_mount(0, &fs);

	LPC_GPIO2->FIODIR |= (1<<0); // red
	LPC_GPIO2->FIODIR |= (1<<1); // green

	LPC_GPIO2->FIOCLR = (1<<0); // red
	LPC_GPIO2->FIOCLR = (1<<1); // green


	RTC_Init(LPC_RTC);

	/* Enable rtc (starts increase the tick counter and second counter register) */
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);
	RTC_CalibCounterCmd(LPC_RTC, DISABLE);

	/* Set current time for RTC */
	// Current time is 8:00:00PM, 2009-04-24
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_SECOND, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MINUTE, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_HOUR, 0);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_MONTH, 12);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_YEAR, 2012);
	RTC_SetTime (LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 19);

	/* Set ALARM time for second */
	RTC_SetAlarmTime (LPC_RTC, RTC_TIMETYPE_SECOND, 10);

}

/**
 * Program entry point 
 */
int main(void) {

	setupHardware();

	xTaskCreate( vFATFSTimerTask		, (signed portCHAR *) "vFATFSTimerTask"		, 100, NULL,		2, NULL );

	xTaskCreate( taskAdc				, (signed portCHAR *) "Adc"				, 1024, NULL,			2, NULL );

	xTaskCreate( taskWebServer			, (signed portCHAR *) "WebServer"		, 2048, NULL,			2, NULL );

	/* Start the scheduler. */
	vTaskStartScheduler();

	/* Will only get here if there was insufficient memory to create the idle task. */
	while(1){

	}

	return 1;
}
