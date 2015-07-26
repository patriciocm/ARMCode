/*
 * Core.c
 *
 *  Created on: Sep 26, 2012
 *      Author: "Facundo Nahuel Uriel Silva"
 */

#include "taskCore.h"
#include "lpc17xx_ssp.h"

void taskCore(void *paramametro){

	const portTickType xDelay = 100 / portTICK_RATE_MS;

//	msgPcCom msgTX;
	msgPcCom msgRX;
	msgPcCom msgTXAck;

	// Se inicializa el mensaje
//	pcComInitMsg(&msgTX.cmd,CMD_PCCOM_READ_ADC_1);
	pcComInitMsg(&msgTXAck.cmd,CMD_PCCOM_ACK);

//	msgTX.cmd.data[0] = 0x00;
//	msgTX.cmd.len = 1;

	while(1){

		// se intenta leer un mensaje de la cola. El numero de elementos en la cola se decrementa en 1
//		if(xQueueReceive(queuePcComRX,&msgRX,100)){

		if(uxQueueMessagesWaiting(queuePcComRX) != 0){

//			xQueueSend(queuePcComTX,&msgTXAck,1000);

			xQueueReceive(queuePcComRX,&msgRX,500);

			switch(msgRX.cmd.cmdId){

				case CMD_PCCOM_ACTUADOR_0_ON:
					LPC_GPIO2->FIOSET = (1<<8);
					break;

				case CMD_PCCOM_ACTUADOR_0_OFF:
					LPC_GPIO2->FIOCLR = (1<<8);
					break;

				case CMD_PCCOM_ACTUADOR_1_ON:
					LPC_GPIO2->FIOSET = (1<<10);
					break;

				case CMD_PCCOM_ACTUADOR_1_OFF:
					LPC_GPIO2->FIOCLR = (1<<10);
					break;

				case CMD_PCCOM_ACTUADOR_2_ON:
					LPC_GPIO2->FIOSET = (1<<7);
					break;

				case CMD_PCCOM_ACTUADOR_2_OFF:
					LPC_GPIO2->FIOCLR = (1<<7);
					break;

				case CMD_PCCOM_ACTUADOR_3_ON:
					LPC_GPIO2->FIOSET = (1<<4);
					break;

				case CMD_PCCOM_ACTUADOR_3_OFF:
					LPC_GPIO2->FIOCLR = (1<<4);
					break;
			}

		}

		taskYIELD();

	}
}
