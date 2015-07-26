/*
 * ProtocolPCComunicaction.h
 *
 *  Created on: Sep 26, 2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#ifndef PROTOCOLPCCOMUNICACTION_H_
#define PROTOCOLPCCOMUNICACTION_H_

#include "LPC17xx.h"
#include "lpc_types.h"

#define MAX_CMDPCCOM_SIZE_DATA	20

typedef struct{
	uint8_t		start;
	uint8_t		cmdId;
	uint8_t		len;
	uint8_t		data[MAX_CMDPCCOM_SIZE_DATA];
	uint16_t	crc;
	uint8_t		end;

} cmdPcCom;

#endif /* PROTOCOLPCCOMUNICACTION_H_ */
