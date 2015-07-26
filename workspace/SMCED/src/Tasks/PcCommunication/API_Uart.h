/*
 * API_Uart.h
 *
 *  Created on: 30/04/2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#ifndef API_UART_H_
#define API_UART_H_

#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_clkpwr.h"

/** \brief
 *
 */
typedef enum{
	UART_PORT_0 = 0,//!< UART_PORT_0 UART 0
	UART_PORT_1 = 1,//!< UART_PORT_1 UART 1
	UART_PORT_2 = 2,//!< UART_PORT_2 UART 2
	UART_PORT_3 = 3 //!< UART_PORT_3 UART 3
	} uartPortNumber;

/** \brief Estructura para modelizar un puerto uart
 *
 */
typedef struct{
		LPC_UART_TypeDef*	port;				//!<
		PINSEL_CFG_Type		pin;				//!<
		UART_CFG_Type		config;				//!<
//		void (*accionInterruption)(uint8_t cr);	//!<
		void (*accionInterruption)();	//!<
		BOOL_8	enabledInterrupt;				//!< flag que indica el estado de la iterrupciones de la uart
		uint8_t byteReceive;
	} uartPort;


/** \brief
 *
 * @param portNumber
 * @param ifh
 */
void API_UartConfig(uartPortNumber portNumber, void(*ifh)(uint8_t cr));

/** \brief Funcion para enviar n bytes por la uart de forma bloqueante
 *
 * @param portNumber
 * @param data
 */
void API_UartSendString(uartPortNumber portNumber,uint8_t* data);

/** \brief
 *
 * @param portNumber
 * @param data
 * @param size
 */
void API_UartSendBytes(uartPortNumber portNumber,uint8_t* data, uint8_t size);

/** \brief
 *
 * @param portNumber
 * @param data
 */
void API_UartSendChar(uartPortNumber portNumber,uint8_t data);

/** \brief
 *
 * @param portNumber
 */
uint8_t API_UartReceiveByte(uartPortNumber portNumber);

/** \brief
 *
 * @param portNumber
 * @return
 */
uint8_t API_UartGetLastByteReceive(uartPortNumber portNumber);

/** \brief
 *
 * @param portNumber
 */
void API_UartEnableInterrupt(uartPortNumber portNumber);

/** \brief
 *
 * @param portNumber
 */
void API_UartDisableInterrupt(uartPortNumber portNumber);


#endif /* API_UART_H_ */
