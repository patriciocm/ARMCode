
#ifndef TASKSDCARD_H_
#define TASKSDCARD_H_

//
//#ifdef __USE_CMSISfs
//#include "LPC17xx.h"
//#endif
//
////#include <cr_section_macros.h>
////#include <NXP/crp.h>
////__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;
//
#include "LPC17xx.h"
#include "lpc_types.h"
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"
//
//#include "../Adc/taskAdc.h"
//#include "../WebServer/taskWebServer.h"
//
//#include "stdio.h"
//#include "stdlib.h"
//#include "string.h"
//
//#include "clocking.h"
#include "ff.h"



/*
 * Just for readability.
 * Writing KHZ(400) or MHZ(10) looks so much better than
 * 400000 or 10000000 in my code.
 */
#define KHZ(x) (x * 1000)
#define MHZ(x) (x * 1000000)


void SetSPIClocks(uint32_t low, uint32_t high);
//
//void TimerReset();
//
//inline void TimerStart(void);
//
//inline void TimerStop(void);
//
//uint32_t TimerRead();
//
///*
// * set_timeout(char *s)
// *
// * Sets the timeout value to be used for calling the disk_timerproc() function.
// */
//static void set_timeout(char *s);
//
///*
// * show_dir()
// *
// * Shows the directory of the SD card.
// * This function is meant as a support function to verify the card contents.
// */
//static void show_dir(char *dirname);
//
///*
// * write_file()
// *
// * Writes a number of records to the file with the given name.
// * This function writes 50 sectors of 512 bytes
// */
//static void write_file(char *fname);
//
///*
// * read_file()
// *
// * Reads the file with the given name.
// * This function reads blocks of 512 bytes
// */
//static void read_file(char *fname);
//
//static void write_log(char *fname);
//
//static void type_file(char *fname);
//
//static void delete_file(char *fname);
//
//
//
//
//#define MAX_SIZE_QUEUE_SDCARD	20
//#define MAX_SIZE_LINE 50	// Numero de caracteres de la linea
//
//
//typedef struct{
//	uint32_t Line;
//} msgSdCardWrite;
//
//typedef struct{
//	uint32_t Line;
//	uint8_t data[MAX_SIZE_LINE];
//} msgSdCardRead;
//
//
///*
// * Scan the given buffer for arguments,
// * split up the buffer and create *args[]
// * to point to each next argument on the line.
// */
//void scan_args(char *buf, char *args[]);
//
void vFATFSTimerTask (void* pvParameters);
//
//void taskWriteSdCard(void *param);
//
//void taskReadSdCard(void *param);

#endif // TASKSDCARD_H_
