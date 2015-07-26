/*
 * PcCommunication.h
 *
 *  Created on: Sep 26, 2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#ifndef PCCOMMUNICATION_H_
#define PCCOMMUNICATION_H_

#include "LPC17xx.h"
#include "lpc_types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "API_Uart.h"
#include "ProtocolPCComunication.h"

#define MAX_SIZE_QUEUE_RX	20
#define MAX_SIZE_QUEUE_TX	20

/** \brief
 *
 */
typedef struct{
	uint8_t		idMsg;
	cmdPcCom	cmd;
} msgPcCom;


/** \brief
 *
 */
void taskPcCommunicationInit();

/** \brief
 *
 * @param param
 */
void taskPcCommunicationTX(void *param);

/** \brief
 *
 * @param param
 */
void taskPcCommunicationRX(void *param);

/** \brief Funcion que se ejecuta al producirce la interrupcion de recepcion de la uart 3
 *
 */
void interruptUart3();
//void interruptUart3(uint8_t c);

#endif /* PCCOMMUNICATION_H_ */
