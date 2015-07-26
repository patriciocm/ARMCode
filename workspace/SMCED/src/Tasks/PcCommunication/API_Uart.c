/*
 * API_Uart.c
 *
 *  Created on: 30/04/2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#include "API_Uart.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

// Variables globales de la API_UART
uartPort control_uarts[4];

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartConfig(uartPortNumber portNumber, void(*ifh)(uint8_t cr)){

	switch (portNumber) {
		case UART_PORT_0:
			control_uarts[portNumber].port = (LPC_UART_TypeDef*)LPC_UART0;
//			LPC_SC->PCONP |= (1<<3) & CLKPWR_PCONP_BITMASK;
			break;
		case UART_PORT_1:
			control_uarts[portNumber].port = (LPC_UART_TypeDef*)LPC_UART1; //  FULL MODEM
//			LPC_SC->PCONP |= (1<<4) & CLKPWR_PCONP_BITMASK;
			break;
		case UART_PORT_2:
			control_uarts[portNumber].port = (LPC_UART_TypeDef*)LPC_UART2;
//			LPC_SC->PCONP |= (1<<24) & CLKPWR_PCONP_BITMASK;
			break;
		case UART_PORT_3:
			control_uarts[portNumber].port = (LPC_UART_TypeDef*)LPC_UART3;
//			LPC_SC->PCONP |= (1<<25) & CLKPWR_PCONP_BITMASK;
			break;
	}

	control_uarts[portNumber].accionInterruption = ifh;

	// Configuracion de los pines de la uart
	control_uarts[portNumber].pin.Funcnum = PINSEL_FUNC_2;
	control_uarts[portNumber].pin.OpenDrain = PINSEL_PINMODE_NORMAL;
	control_uarts[portNumber].pin.Pinmode = PINSEL_PINMODE_PULLUP;
	control_uarts[portNumber].pin.Pinnum = PINSEL_PIN_0; //TXD3
	control_uarts[portNumber].pin.Portnum = PINSEL_PORT_0;

	PINSEL_ConfigPin(&control_uarts[portNumber].pin);
	control_uarts[portNumber].pin.Pinnum = PINSEL_PIN_1;
	PINSEL_ConfigPin(&control_uarts[portNumber].pin); //RXD3

	// Configuracion de la UART
	control_uarts[portNumber].config.Baud_rate = 115200;
	control_uarts[portNumber].config.Databits = UART_DATABIT_8;
	control_uarts[portNumber].config.Parity = UART_PARITY_NONE;
	control_uarts[portNumber].config.Stopbits = UART_STOPBIT_1;

	UART_Init(control_uarts[portNumber].port ,&control_uarts[3].config);

//	UART_TxCmd(control_uarts[portNumber].port, ENABLE);

	control_uarts[portNumber].port->TER |= (1<<7); /*!< Transmit enable bit */
	control_uarts[portNumber].port->IER |= (1<<0); /*!< RBR Interrupt enable*/
//	control_uarts[portNumber].port->IER |= (1<<1); /*!< THR Interrupt enable*/

//	UART_IntConfig(control_uarts[portNumber].port, UART_INTCFG_RBR, ENABLE); // Se activa la interrupcion de RXD

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartSendString(uartPortNumber portNumber,uint8_t* data){

	int i;
	int lenData = strlen((char*)data);
	for(i = 0; i < lenData; i++){
		API_UartSendChar(portNumber, data[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartSendBytes(uartPortNumber portNumber,uint8_t* data, uint8_t size){

	int i;
	for(i = 0; i < size; i++){
		API_UartSendChar(portNumber, data[i]);
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartSendChar(uartPortNumber portNumber,uint8_t data){

	control_uarts[portNumber].port->THR = data;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

uint8_t API_UartReceiveByte(uartPortNumber portNumber){

	control_uarts[portNumber].byteReceive = control_uarts[portNumber].port->RBR;

	return control_uarts[portNumber].byteReceive;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

uint8_t API_UartGetLastByteReceive(uartPortNumber portNumber){

	return control_uarts[portNumber].byteReceive;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartEnableInterrupt(uartPortNumber portNumber){

	switch(portNumber){
		case UART_PORT_0:
			NVIC_EnableIRQ(UART0_IRQn);
			break;

		case UART_PORT_1:
			NVIC_EnableIRQ(UART1_IRQn);
			break;

		case UART_PORT_2:
			NVIC_EnableIRQ(UART2_IRQn);
			break;

		case UART_PORT_3:
			NVIC_EnableIRQ(UART3_IRQn);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void API_UartDisableInterrupt(uartPortNumber portNumber){

	switch(portNumber){
		case UART_PORT_0:
			NVIC_DisableIRQ(UART0_IRQn);
			break;

		case UART_PORT_1:
			NVIC_DisableIRQ(UART1_IRQn);
			break;

		case UART_PORT_2:
			NVIC_DisableIRQ(UART2_IRQn);
			break;

		case UART_PORT_3:
			NVIC_DisableIRQ(UART3_IRQn);
			break;
	}
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
// Handlers de las interrupciones de las uarts
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void UART0_IRQHandler(void){

	if(control_uarts[UART_PORT_0].accionInterruption != NULL)
		control_uarts[UART_PORT_0].accionInterruption();
//		control_uarts[UART_PORT_0].accionInterruption(control_uarts[UART_PORT_0].port->RBR);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void UART1_IRQHandler(void){

	if(control_uarts[UART_PORT_1].accionInterruption != NULL)
		control_uarts[UART_PORT_1].accionInterruption();
//		control_uarts[UART_PORT_1].accionInterruption(control_uarts[UART_PORT_1].port->RBR);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void UART2_IRQHandler(void){

	if(control_uarts[UART_PORT_2].accionInterruption != NULL)
		control_uarts[UART_PORT_2].accionInterruption();
//	control_uarts[UART_PORT_2].accionInterruption(control_uarts[UART_PORT_2].port->RBR);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void UART3_IRQHandler(void){

	if(control_uarts[UART_PORT_3].accionInterruption != NULL)
		control_uarts[UART_PORT_3].accionInterruption();
//		control_uarts[UART_PORT_3].accionInterruption(control_uarts[UART_PORT_3].port->RBR);

}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
