/*
 * Core.h
 *
 *  Created on: Sep 26, 2012
 *      Author: "Facundo Nahuel Uriel Silva"
 */

#ifndef CORE_H_
#define CORE_H_

#include "LPC17xx.h"
#include "lpc_types.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Tasks/PcCommunication/taskPcCommunication.h"
#include "Tasks/PcCommunication/ProtocolPCComunication.h"

extern xQueueHandle queuePcComTX;
extern xQueueHandle queuePcComRX;

void taskCore(void *paramametro);

#endif /* CORE_H_ */
