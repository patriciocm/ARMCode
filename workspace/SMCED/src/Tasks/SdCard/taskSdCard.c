///*
// * FatFS Test program
// *
// * This program contains a number of tests for ChaN's FatFS
// * on the lpc1769 module
// *
// * The idea is to create a generic test suite that can be used to
// * check if the SD card interface works.
// * Also, a timer is used to measure the amount of time needed by
// * different functions to check the timing.
// *
// * (c) Rob Jansen
// *
// * This software is free software, you may use it, enhance it
// * and share it as long as you like as long as this copyright
// * message stays in place.
// */
//
#include "taskSdCard.h"
//
//extern xQueueHandle queueAdcSdCard;
//
//xQueueHandle queueSdCardWrite;
//xQueueHandle queueSdCardRead;
//
//xSemaphoreHandle semSdCardAccess;
//xSemaphoreHandle semSdCardReadAction;
//
//extern char bufferReadSdCard[50];
//
//extern FATFS fs;
//
uint8_t spi_div_low = 250;
uint8_t spi_div_high = 10;
//
//extern uint32_t timerproc_interval;
//

void vFATFSTimerTask (void* pvParameters)
{
	portTickType xLastWakeTime = xTaskGetTickCount();
	while(1){
		disk_timerproc();
		vTaskDelayUntil (&xLastWakeTime, (10 / portTICK_RATE_MS)); //each 10ms
	}
}


///*
// * //printf() stuff.
// * Use small_//printf and create a buffer for one line of text.
// * The define makes sure that I can use //printf() as if nothing happened.
// */
//#define STRLEN 255
//#define N_ARGS    2
//char str[STRLEN];
//char line[STRLEN];
//char *args[N_ARGS];
//
//int8_t volatile Buffer[512]; /* Buffer to store a complete sector */
//
///*
// * static void SetSPIClocks(low, high)
// *
// * Sets the divider values to create the low and high SPI clocks
// * that are used by the SD card driver.
// *
// * Note that the divier values are rounded up to the first even
// * value to generate a valid divider values that results in the
// * closest frequency that is not higher than the wanted frequency.
// */

void SetSPIClocks(uint32_t low, uint32_t high){
	spi_div_low  = SystemCoreClock / low;
	spi_div_high = (SystemCoreClock+high-1) / high;

	if(spi_div_low & 1) spi_div_low++;
	if(spi_div_high & 1) spi_div_high++;
}

///*
// * Timer functions
// *
// * These functions use timer 0 to create a stopwatch with a 1 us resolution.
// * Generally these functions will be used in the same way as a regular stopwatch
// * is being used: first reset, then start, then stop and then read.
// * It is possible to read the timer while it is running to get intermediate
// * results although this is not the general way in which I intent to use it.
// *
// * Please note that there is some overhead in starting and stopping the timer.
// *
// */
//void TimerReset()
//{
//	LPC_TIM0->TCR = 2;  /* Reset timer */
//	LPC_TIM0->CTCR = 0; /* Timer mode  */
//	LPC_TIM0->PR = SystemCoreClock/MHZ(1);
//	LPC_TIM0->TCR = 0;
//}
//
//inline void TimerStart(void)
//{
//	LPC_TIM0->TCR = 1;
//}
//
//inline void TimerStop(void)
//{
//	LPC_TIM0->TCR = 0;
//}
//
//uint32_t TimerRead()
//{
//	return LPC_TIM0->TC;
//}
//
///*
// * set_timeout(char *s)
// *
// * Sets the timeout value to be used for calling the disk_timerproc() function.
// */
//static void set_timeout(char *s)
//{
//	uint32_t timeout=0;
//
//	while((*s >= '0') && (*s <= '9'))
//	{
//		timeout = (timeout * 10) + (*s - '0');
//		s++;
//	}
//
//	if(timeout > 5)
//	{
//		timerproc_interval = timeout;
//		//printf("Changed 10 ms interval to %u ms\n", timeout);
//	}
//	else
//	{
//		//printf("Invalid interval: not changed\n");
//	}
//}
//
///*
// * show_dir()
// *
// * Shows the directory of the SD card.
// * This function is meant as a support function to verify the card contents.
// */
//static void show_dir(char *dirname)
//{
//	FRESULT res;
//	DIR dir;
//	FILINFO fileinfo;
//
//	res = f_opendir(&dir, "/");
//	if (res)
//	{
//		//show_res("Cannot open dir", res);
//		return;
//	}
//
//	for(;;)
//	{
//		res = f_readdir(&dir, &fileinfo);
//		if((res != FR_OK) || !fileinfo.fname[0])
//			break;
////		//printf("%c%c%c%c%c %u/%02u/%02u %02u:%02u %9lu  %s\n",
////				(fileinfo.fattrib & AM_DIR) ? 'D' : '-',
////				(fileinfo.fattrib & AM_RDO) ? 'R' : '-',
////				(fileinfo.fattrib & AM_HID) ? 'H' : '-',
////				(fileinfo.fattrib & AM_SYS) ? 'S' : '-',
////				(fileinfo.fattrib & AM_ARC) ? 'A' : '-',
////				(fileinfo.fdate >> 9) + 1980, (fileinfo.fdate >> 5) & 15, fileinfo.fdate & 31,
////				(fileinfo.ftime >> 11), (fileinfo.ftime >> 5) & 63,
////				fileinfo.fsize, &(fileinfo.fname[0]));
//
//	}
//}
//
///*
// * write_file()
// *
// * Writes a number of records to the file with the given name.
// * This function writes 50 sectors of 512 bytes
// */
//static void write_file(char *fname)
//{
//	FIL file;
//	FRESULT res;
//	UINT written;
//	uint32_t i, idx,bufferSize;
//
//	res = f_open(&file, fname, FA_CREATE_NEW | FA_WRITE);
//	if(res != FR_OK){
//		return;
//	}
//
//	sprintf(Buffer, "Prueba desde Free RTOS...");
//	bufferSize = strlen(Buffer);
//
//	TimerReset();
//	LPC_GPIO0->FIOSET |= 1<<22;
//	TimerStart();
//	res = f_write(&file, Buffer, bufferSize, &written);
//	TimerStop();
//	LPC_GPIO0->FIOCLR |= 1<<22;
//
//	res = f_close(&file);
//
//}
//
///*
// * read_file()
// *
// * Reads the file with the given name.
// * This function reads blocks of 512 bytes
// */
//static void read_file(char *fname)
//{
//	FIL file;
//	FRESULT res;
//	UINT read;
//	uint32_t sum_read;
//	uint32_t i, idx;
//	uint32_t data_err;
//
//
//	res = f_open(&file, fname, FA_READ);
//	if(res != FR_OK)
//	{
//		//show_res("Cannot open file for reading", res);
//		return;
//	}
//
//	sum_read = 0;
//	i=0;
//	data_err = 0;
//
//	while(!f_eof(&file))
//	{
//		TimerReset();
//		LPC_GPIO0->FIOSET |= 1<<22;
//		TimerStart();
//		res = f_read(&file, Buffer, 512, &read);
//		TimerStop();
//		LPC_GPIO0->FIOCLR |= 1<<22;
//
//		sum_read += read;
//
//		if(res != FR_OK)
//		{
//			//show_res("Error reading file", res);
//			break;
//		} else
//		{
//			/*
//			 * Check data
//			 */
//			for(idx=0; idx<read; idx++)
//			{
//				if(Buffer[idx] != (((i * 3) + (idx * 5)) & 0xff)) data_err++;
//			}
//			i++;
//
//			if(read == 512)
//			{
//				//printf("Time: %u us\n", TimerRead());
//			}
//			else if(!f_eof(&file))
//			{
//				//printf("Time: %u us, read incomplete - read %d bytes\n", TimerRead(), (int)read);
//			} else
//			{
//				//printf("Time: %u us, end of file reached total %u bytes read\n", TimerRead(), (int)sum_read);
//			}
//		}
//	}
//	if(data_err) //printf("Error: data does not match expected pattern\n");
//
//	res = f_close(&file);
////	if(res != FR_OK)
//		//show_res("Error closing file", res);
//}
//
//
//static void write_log(char *fname)
//{
//	FIL file;
//	FRESULT res;
//	UINT written;
//	uint32_t i;
//	uint32_t write_time, sync_time;
//	char logstr[80];
//
//	res = f_open(&file, fname, FA_CREATE_NEW | FA_WRITE);
//	if(res != FR_OK)
//	{
//		//show_res("Cannot open file for writing", res);
//		return;
//	}
//
//	for(i=0; i<250; i++)
//	{
//		//printf(logstr, 80, "Logging some data. SysTick->Val = %u\n\r", SysTick->VAL);
//
//		TimerReset();
//		LPC_GPIO0->FIOSET |= 1<<22;
//		TimerStart();
//		res = f_write(&file, logstr, strlen(logstr), &written);
//		TimerStop();
//		LPC_GPIO0->FIOCLR |= 1<<22;
//		write_time = TimerRead();
//
//		TimerReset();
//		TimerStart();
//		f_sync(&file);
//		TimerStop();
//		sync_time = TimerRead();
//
//		if(res != FR_OK)
//		{
//			//show_res("Error writing", res);
//			break;
//		} else
//		{
//			if(written == strlen(logstr))
//			{
//				//printf("Time: write %u us, sync %u us\n", write_time, sync_time);
//			}
//			else
//			{
//				//printf("Time: write %u us, sync %u us, incomplete - wrote %d bytes\n", write_time, sync_time, (int) written);
//			}
//		}
//	}
//	res = f_close(&file);
////	if(res != FR_OK)
//		//show_res("Error closing file", res);
//	//printf("\n");
//}
//
//static void type_file(char *fname)
//{
//	FIL file;
//	FRESULT res;
//	UINT read;
//
//
//	res = f_open(&file, fname, FA_READ);
//	if(res != FR_OK)
//	{
//		//show_res("Cannot open file for reading", res);
//		return;
//	}
//
//	while(!f_eof(&file))
//	{
//		res = f_read(&file, Buffer, 256, &read);
//
//		if(res != FR_OK)
//		{
//			//show_res("Error reading file", res);
//			break;
//		} else
//		{
////			UARTSend(3, Buffer, read);
//		}
//	}
//	res = f_close(&file);
////	if(res != FR_OK)
//		//show_res("Error closing file", res);
//	//printf("\n");
//}
//
//static void delete_file(char *fname)
//{
//	FRESULT res;
//
//	res = f_unlink(fname);
//
//	if(res != FR_OK){
//		//show_res("Error deleting file", res);
//	}else
//	{
//		//printf("File %s deleted\n", fname);
//	}
//
//	return;
//}
//
////int get_line (		/* 0:End of stream, 1:A line arrived */
////	char* buff,		/* Pointer to the buffer */
////	int len			/* Buffer length */
////)
////{
////	int rv, c, i;
////
////	UARTPutChar(3, '\r');
////	UARTPutChar(3, '>');
////
////	rv = i = 0;
////	for (;;) {
////		c = UARTGetChar(3);			/* Get a char from the incoming stream   */
////		if(c == 0x7f) c = '\b';		/* Rob: putty uses DEL for backspace ... */
////		if (!c) return 0;			/* End of stream? */
////		if (c == '\r') break;		/* End of line? */
////		if (c == '\b' && i) {		/* Back space? */
////			i--;
////			UARTPutChar(3, c);
////			UARTPutChar(3, ' ');
////			UARTPutChar(3, c);
////			continue;
////		}
////		if (c >= ' ' && i < len - 1) {	/* Visible chars */
////			buff[i++] = c;
////			UARTPutChar(3, c);
////		}
////	}
////	buff[i] = 0;	/* Terminate with zero */
////	UARTPutChar(3, '\n');
////	UARTPutChar(3, '\r');
////
////	return 1;
////}
//
///*
// * Scan the given buffer for arguments,
// * split up the buffer and create *args[]
// * to point to each next argument on the line.
// */
//void scan_args(char *buf, char *args[])
//{
//	int i;
//
//	for(i=0; i<N_ARGS; i++) args[i] = 0L;
//
//	i=0;
//	while(*buf && (*buf != ' ')) buf++; /* Skip the command    */
//	while(*buf == ' ') buf++;           /* and skip any spaces */
//	while(*buf && (i < N_ARGS))
//	{
//		args[i] = buf;
//		while(*buf && (*buf != ' ')) buf++; /* Forward to the next white space */
//		while(*buf == ' ') buf++;           /* and skip the spaces             */
//		i++;
//	}
//}
//
//void taskWriteSdCard(void *param){
//
//	msgAdcSdCard msgAdcSD;
//	uint8_t dataTemp[20];
//	int i;
//
//	FIL file;
//	FRESULT res;
//	UINT written;
//	uint32_t bufferSize;
//
//	// Poner al principio
//	res = f_mount(0, &fs);
//	if(res != FR_OK){
//		vTaskDelete(NULL);
//	}
//
//	sprintf(Buffer, "V;I;\n");
//	bufferSize = strlen(Buffer);
//
////	taskDISABLE_INTERRUPTS();
////	taskENTER_CRITICAL();
//	res = f_open(&file, SAMPLES_FILE, FA_CREATE_ALWAYS | FA_WRITE);
//	if(res != FR_OK){
//		vTaskDelete(NULL);
//	}
//
//	res = f_write(&file, Buffer, bufferSize, &written);
//	res = f_close(&file);
//
////	taskEXIT_CRITICAL();
////	taskENABLE_INTERRUPTS();
//
////	xSemaphoreGive(semSdCardAccess);
//
//	while(1)
//	{
////		xSemaphoreTake(semSdCardAccess,portMAX_DELAY);
//
//		if(uxQueueMessagesWaiting(queueAdcSdCard) != 0){
//
//			xQueueReceive(queueAdcSdCard,&msgAdcSD,500);
//
//			memset(Buffer,0x00, 512);
//
//			for(i=0; i < SIZE_SAMPLES; i++){
//				sprintf(dataTemp, "%d;%d;\n", msgAdcSD.values[i].Adc0Value, 	msgAdcSD.values[i].Adc1Value );
//				strcat(Buffer, dataTemp);
//			}
//
//			bufferSize = strlen(Buffer);
//
////			taskDISABLE_INTERRUPTS();
////			taskENTER_CRITICAL();
//			res = f_open(&file, SAMPLES_FILE,FA_WRITE);
//
//			if(res != FR_OK){
//				vTaskDelete(NULL);
//			}
//
//			f_lseek(&file,file.fsize);
//			res = f_write(&file, Buffer, bufferSize, &written);
//			res = f_close(&file);
////			taskEXIT_CRITICAL();
////			taskENABLE_INTERRUPTS();
//
//		}
//
////		xSemaphoreGive(semSdCardAccess);
//
//	}
//}
//
//void taskReadSdCard(void *param){
//
//	msgSdCardRead msgSDRead;
//	FIL file;
//	FRESULT res;
//	UINT written;
//	uint32_t i, idx,bufferSize;
//
//	while(1){
//
//		xSemaphoreTake(semSdCardAccess,portMAX_DELAY);
//
//		if(uxQueueMessagesWaiting(queueSdCardRead) != 0){
//
//			xQueueReceive(queueSdCardRead,&msgSDRead,500);
//			xSemaphoreGive(semSdCardReadAction);
//
//			strcpy(bufferReadSdCard, "Funco!!!");
//		}
//
////		xSemaphoreGive(semSdCardAccess);
//		taskYIELD();
//	}
//}


