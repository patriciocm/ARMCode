/*
 * ProtocolPCComunicaction.c
 *
 *  Created on: Sep 26, 2012
 *      Author: Facundo Nahuel Uriel Silva
 */

#include "ProtocolPCComunication.h"

int countRxCmdDataLen;

int pcComMachineState(cmdPcCom* cmd, stateReceptionMsg* state, unsigned char byteReceive){

	int result = PCCOM_RX_MSG_IN_PROCESS;

	switch(*state){

		case SRM_RECEIVING_START:

			if(byteReceive == PCCOM_START_BYTE){
				*state = SRM_RECEIVING_CMD_TYPE;
				cmd->start = byteReceive;
			}

			break;

		case SRM_RECEIVING_CMD_TYPE:

			*state = SRM_RECEIVING_DATA_LEN;
			cmd->cmdId = byteReceive;

			break;

		case SRM_RECEIVING_DATA_LEN:

			if(byteReceive == 0x00){
				*state = SRM_RECEIVING_CRC_1;
			}
			else{
				*state = SRM_RECEIVING_CMD_DATA;
				countRxCmdDataLen = 0; // se inica en conteo de bytes recividos
			}

			cmd->len = byteReceive;

			break;

		case SRM_RECEIVING_CMD_DATA:

			cmd->data[countRxCmdDataLen] = byteReceive;
			countRxCmdDataLen++;

			if(countRxCmdDataLen == cmd->len){
				*state = SRM_RECEIVING_CRC_1;
			}

			break;

		case SRM_RECEIVING_CRC_1:

                        cmd->crcHL[0] = byteReceive;
			*state = SRM_RECEIVING_CRC_2;

			break;

		case SRM_RECEIVING_CRC_2:

//			cmd->crc &= (byteReceive << 8);
                        cmd->crcHL[1] = byteReceive;
			*state = SRM_RECEIVING_END;

			break;

		case SRM_RECEIVING_END:

			if(byteReceive == PCCOM_END_BYTE){
				*state = SRM_RECEIVING_START;
				result = PCCOM_RX_MSG_TERMINATE;
			}
			cmd->end = byteReceive;

			break;
	}

	return result;

}

void pcComInitMsg(cmdPcCom* msg, unsigned char cmd){

	msg->start = PCCOM_START_BYTE;
	msg->cmdId = cmd;
	msg->len = 0x00;
	memset(msg->data, 0x00, MAX_CMDPCCOM_SIZE_DATA);
	msg->crc = 0x0000;
	msg->end = PCCOM_END_BYTE;

}

unsigned char pcComPackMsg(cmdPcCom *msg, unsigned char *buffer, unsigned char lenBuffer){

	unsigned char lenPackedMsg = 0;

	memset(buffer, 0x00, lenBuffer);

	if(msg != NULL && buffer != NULL){ // Se verifican los punteros

		if(lenBuffer >= (MIN_CMDPCCOM_SIZE_PACKED + msg->len)){ // Se verifica que el largo del buffer sea valido

			memset(buffer, msg->start, 1);
			lenPackedMsg++;

			memset(buffer + lenPackedMsg, msg->cmdId, 1);
			lenPackedMsg++;

			memset(buffer + lenPackedMsg, msg->len, 1);
			lenPackedMsg++;

			memcpy(buffer + lenPackedMsg, msg->data, msg->len);
			lenPackedMsg += msg->len;

			// Se calcula el crc
			msg->crc = crc16(buffer, lenPackedMsg);

			memcpy(buffer + lenPackedMsg, msg->crc, 2);
			lenPackedMsg += 2;

			memset(buffer + lenPackedMsg, msg->end, 1);
			lenPackedMsg++;
		}
	}

	return lenPackedMsg;

}

/**
*                                       16   12   5
* this is the CCITT CRC 16 polynomial X  + X  + X  + 1.
* This works out to be 0x1021, but the way the algorithm works
* lets us use 0x8408 (the reverse of the bit pattern).  The high
* bit is always assumed to be set, thus we only use 16 bits to
* represent the 17 bit value.
*
* Fuente: http://www8.cs.umu.se/~isak/snippets/crc-16.c
*/

unsigned short crc16(unsigned char *data_p, unsigned short length)
{
      unsigned char i;
      unsigned int data;
      unsigned int crc = 0xffff;

      if (length == 0)
            return (~crc);

      do
      {
            for (i=0, data=(unsigned int)0xff & *data_p++;
                 i < 8;
                 i++, data >>= 1)
            {
                  if ((crc & 0x0001) ^ (data & 0x0001))
                        crc = (crc >> 1) ^ POLY;
                  else  crc >>= 1;
            }
      } while (--length);

      crc = ~crc;
      data = crc;
      crc = (crc << 8) | (data >> 8 & 0xff);

      return (crc);
}
