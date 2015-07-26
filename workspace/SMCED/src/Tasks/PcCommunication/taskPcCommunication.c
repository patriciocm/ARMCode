/*
 * PcCommunication.c
 *
 *  Created on: Sep 26, 2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#include "taskPcCommunication.h"

///////////////////////////////////////////////////////////////////////////////////////////////
// Colas de mensajes
///////////////////////////////////////////////////////////////////////////////////////////////

xQueueHandle queuePcComTX;
xQueueHandle queuePcComRX;

xSemaphoreHandle semComRx;

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void taskPcCommunicationInit(){

	// Se inicializa la uart 3
	API_UartConfig(UART_PORT_3, interruptUart3);

	// Se inicializa la cola de mensajes de transmision
	queuePcComTX = xQueueCreate( MAX_SIZE_QUEUE_TX, sizeof( msgPcCom ) );

	// Se inicializa la cola de mensajes de recepcion
	queuePcComRX = xQueueCreate( MAX_SIZE_QUEUE_RX, sizeof( msgPcCom ) );

	semComRx = xSemaphoreCreateMutex();

	xSemaphoreTake(semComRx,portMAX_DELAY);

	API_UartEnableInterrupt(UART_PORT_3);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void taskPcCommunicationTX(void *param){

	const portTickType xDelay = 30 / portTICK_RATE_MS;
	msgPcCom	currentMsgTx;
	uint8_t bufferSend[MAX_CMDPCCOM_SIZE_DATA + MIN_CMDPCCOM_SIZE_PACKED];
	uint8_t sizeBufferSend = 0;

	while (1) {

		if(uxQueueMessagesWaiting(queuePcComTX) != 0){ // se espera hasta que la cola tenga mensajes

			// se intenta leer un mensaje de la cola. El numero de elementos en la cola se decrementa en 1
			if(xQueueReceive(queuePcComTX,&currentMsgTx,500)){

				// Se carga buffer con el mensaje a enviar
				sizeBufferSend = pcComPackMsg(&currentMsgTx.cmd,bufferSend, MAX_CMDPCCOM_SIZE_DATA + MIN_CMDPCCOM_SIZE_PACKED);

				API_UartSendBytes(UART_PORT_3,bufferSend, sizeBufferSend);
			}

			vTaskDelay( xDelay );
		}

		taskYIELD();

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

void taskPcCommunicationRX(void *param){

	uint8_t		byteReceive; // byte recibido
	msgPcCom	currentMsgRx;
	stateReceptionMsg stateMsgRx = SRM_RECEIVING_START;
	int result = 0;
	currentMsgRx.idMsg = 0;

	while (1) {

		if(xSemaphoreTake(semComRx,portMAX_DELAY) == pdTRUE){ // se espera a que llegue algo

			byteReceive = API_UartGetLastByteReceive(UART_PORT_3);

			result = pcComMachineState(&currentMsgRx.cmd, &stateMsgRx, byteReceive);

			// Se se puedo cargar el mensaje se encola
			if(result == PCCOM_RX_MSG_TERMINATE){
				currentMsgRx.idMsg++;
				xQueueSend(queuePcComRX,&currentMsgRx,portMAX_DELAY);
			}
		}

//		taskYIELD();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

//void interruptUart3(uint8_t c){
void interruptUart3(){

	signed portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	API_UartDisableInterrupt(UART_PORT_3);
	// Se lee el byte recivido
	API_UartReceiveByte(UART_PORT_3);

	// Se cede el semaforo para indicar que llego un byte desde la uart
	xSemaphoreGiveFromISR(semComRx,&pxHigherPriorityTaskWoken);

	API_UartEnableInterrupt(UART_PORT_3);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
