
#ifndef TASKWEB_H_
#define TASKWEB_H_

#include <stdio.h>
#include <string.h>

#include "LPC17xx.h"
#include "lpc_types.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "../ezxml.h"
#include "../Tasks/SdCard/ff.h"
#include "../Tasks/Adc/taskAdc.h"

extern FATFS fs;

unsigned char *PWebSide;                         // pointer to webside
unsigned int HTTPBytesToSend;                    // bytes left to send

unsigned char HTTPStatus;                        // status byte
#define HTTP_SEND_PAGE               0x01        // help flag



void taskWebServer (void *param);

void taskTcpTick(void *param);

#endif //TASKWEB_H_
