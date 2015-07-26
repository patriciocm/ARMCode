
#include "taskWebServer.h"

// CodeRed - added #define extern on next line (else variables
// not defined). This has been done due to include the .h files
// rather than the .c files as in the original version of easyweb.
//#include "easyweb.h"
#define extern
#include "ethmac.h"
#include "tcpip.h"


#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;


extern xQueueHandle queueSdCardWrite;
extern xQueueHandle queueSdCardRead;

extern xSemaphoreHandle semSdCardReadAction;

char bufferReadSdCard[50];

// IP: 192.168.1.110

void taskWebServer (void *param)
{
	ezxml_t xml;
	int8_t* ptrAux;
	int8_t* ptrAuxIni;
	int8_t* ptrAuxEnd;
	int8_t tempBuffer[50];
	int8_t tempFile[1024];

	int32_t bytesRead;
	int32_t i;

	FIL file;
	FRESULT res;

//	msgSdCardRead msgSdCardR;
//	msgSdCardWrite msgSdCardW;

//	semWebSD = xSemaphoreCreateMutex();
//	xSemaphoreTake(semWebSD,portMAX_DELAY);

	TCPLowLevelInit();
	xTaskCreate( taskTcpTick, (signed portCHAR *) "taskTcpTick"	, 240, NULL, 2, NULL );

	HTTPStatus = 0;                                // clear HTTP-server's flag register

	TCPLocalPort = TCP_PORT_HTTP;                  // set port we want to listen to


	while (1)                                      // repeat forever
	{
		if (!(SocketStatus & SOCK_ACTIVE)){						// listen for incoming TCP-connection
			TCPPassiveOpen();
		}

		NetworkEngine();

		///////////////////////////////

		if (SocketStatus & SOCK_CONNECTED){             // check if somebody has connected to our TCP

			if (SocketStatus & SOCK_DATA_AVAILABLE){      // check if remote TCP sent data

				memset(TCP_TX_BUF,'\0',sizeof(TCP_TX_BUF)); // Se limpia el buffer de salida

				if (SocketStatus & SOCK_TX_BUF_RELEASED){     // check if buffer is free for TX

					// Para acceder poner en el browser http://192.168.1.110:80
					if(TCPLocalPort == 80){

					/////////////////////////////////////////////////////
					// ENCABEZADO
					/////////////////////////////////////////////////////
					strcpy(TCP_TX_BUF, "HTTP/1.0 200 OK\r\n");
					strcat(TCP_TX_BUF, "Content-Type: text/html\r\n");
					strcat(TCP_TX_BUF, "\r\n");


					//////////////////////////////////////
					// Se recupera el destino solicitado
					//////////////////////////////////////

					memset(tempBuffer, 0x00, 50);
					memset(tempFile, 0x00, 200);

					ptrAuxIni = strstr(TCP_RX_BUF,"GET /");
					ptrAuxIni += strlen("GET /");
					ptrAuxEnd = strstr(ptrAuxIni," ");

					ptrAux = strncpy(tempBuffer,ptrAuxIni, ptrAuxEnd - ptrAuxIni);
					tempBuffer[ptrAuxEnd - ptrAuxIni] = '\0';


					if( !strcmp(tempBuffer,"mediciones.hml") ){ // pagina dinamica

						strcat(TCP_TX_BUF, "<meta http-equiv='refresh' content='");
						sprintf(tempBuffer,"%d",NUMERO_SEGUNDOS_GUARDAR_VALORES);
						strcat(TCP_TX_BUF, tempBuffer);
						strcat(TCP_TX_BUF, "' >\r\n");

						strcat(TCP_TX_BUF, "<html>\r\n");
							strcat(TCP_TX_BUF, "<title>SMCED</title>\r\n");
							strcat(TCP_TX_BUF, "<body style='width:750px;margin:25px auto;'>\r\n");

							strcat(TCP_TX_BUF, "<form>\r\n");
								strcat(TCP_TX_BUF, "<b>Mediciones</b><br>");
								strcat(TCP_TX_BUF, "<a href='");
								strcat(TCP_TX_BUF, MEDICION_FILE);
								strcat(TCP_TX_BUF, "'>Descargar Historial</a><br><br>");

								res = f_open(&file, MEDICION_FILE , FA_READ);

								if(res == FR_OK){
									res = f_read(&file, tempFile, sizeof(tempFile), &bytesRead);
									res = f_close(&file);
									tempFile[bytesRead - 1] = '\0';

									strcat(TCP_TX_BUF, "<div style='border:solid 2px black;width:655px;height:500px;overflow-y:scroll;'>\n");

										if(strlen(tempFile) > strlen(HEADER_CSV_FILE)){

											strcat(TCP_TX_BUF, "<table border=1>\n");

											strcat(TCP_TX_BUF, "<tr>\n");
												strcat(TCP_TX_BUF, "<td><b>TimeStamp</b></td>\n");
												strcat(TCP_TX_BUF, "<td><b>Tension[V]</b></td>\n");
												strcat(TCP_TX_BUF, "<td><b>Corriente[A]</b></td>\n");
												strcat(TCP_TX_BUF, "<td><b>Factor Potencia</b></td>\n");
												strcat(TCP_TX_BUF, "<td><b>Frecuencia[Hz]</b></td>\n");
												strcat(TCP_TX_BUF, "<td><b>Consumo[W/h]</b></td>\n");
											strcat(TCP_TX_BUF, "</tr>\n");

											ptrAuxIni = tempFile;

											// Se saltea el encabezado del archivo csv
											for(i = 0; i<5;i++){
												ptrAuxIni = strstr(ptrAuxIni,";") + 1;
											}

											while( (ptrAuxEnd = strstr(ptrAuxIni,";") ) != NULL){

												strcat(TCP_TX_BUF, "<tr>");

	//											ptrAuxEnd = strstr(ptrAuxIni,";");
	//
	//											strncpy(tempBuffer,ptrAuxIni,ptrAuxEnd - ptrAuxIni);
	//											tempBuffer[ptrAuxEnd - ptrAuxIni] = '\0';
	//
	//											ptrAuxIni = ptrAuxEnd++;
	//											strcat(TCP_TX_BUF, "<td>");
	//											strcat(TCP_TX_BUF, tempBuffer);
	//											strcat(TCP_TX_BUF, "</td>");

												for(i=0; i < 6; i++){

													ptrAuxIni++;
													ptrAuxEnd = strstr(ptrAuxIni,";");

													strncpy(tempBuffer,ptrAuxIni,ptrAuxEnd - ptrAuxIni);
													tempBuffer[ptrAuxEnd - ptrAuxIni] = '\0';

													ptrAuxIni = ptrAuxEnd++;
													strcat(TCP_TX_BUF, "<td>");
													strcat(TCP_TX_BUF, tempBuffer);
													strcat(TCP_TX_BUF, "</td>");
												}

												strcat(TCP_TX_BUF, "</tr>");
												ptrAuxIni++;
											}

											strcat(TCP_TX_BUF, "</table>\n");
										}else{
											strcat(TCP_TX_BUF, "Valores no diponibles, espere.");
											strcat(TCP_TX_BUF, "\r\n");
										}
									strcat(TCP_TX_BUF, "</div>\n");
								strcat(TCP_TX_BUF, "</form>");
							strcat(TCP_TX_BUF, "</body>");
						strcat(TCP_TX_BUF, "</html>");



							}else{
								strcat(TCP_TX_BUF, "Valores no diponibles, espere.");
								strcat(TCP_TX_BUF, "\r\n");
							}


					}else{

//						res = f_chdir("WEB");

						if( tempBuffer[0] == '\0'){ // se pido el index.hml
							sprintf(tempBuffer,"INDEX.HML");
						}

						res = f_open(&file, tempBuffer, FA_READ);

						if(res == FR_OK){

							do{
								f_read(&file, tempFile, sizeof(tempFile), &bytesRead);
								tempFile[bytesRead] = '\0';
								if(bytesRead != 0)
									strcat(TCP_TX_BUF, tempFile);
							}while(bytesRead != 0);

							f_close(&file);

						}else{

							strcat(TCP_TX_BUF, "<html>\r\n");
								strcat(TCP_TX_BUF, "<form>\r\n");
									strcat(TCP_TX_BUF, "<b>404 - File Not Found</b>\r\n");
							strcat(TCP_TX_BUF, "</form>");
						}

//						res = f_chdir("..");

					}

				}

					//				// Para acceder poner en el browser http://192.168.1.110:90
					//				if(TCPLocalPort == 90){
					//



					TCPTxDataCount = strlen(TCP_TX_BUF);   // bytes to xfer

					TCPTransmitTxBuffer();                   // xfer buffer
					TCPClose();                              // and close connection
					HTTPBytesToSend = 0;                     // all data sent

					HTTPStatus |= HTTP_SEND_PAGE;              // ok, 1st loop executed
				}
			}
		}else{
			HTTPStatus &= ~HTTP_SEND_PAGE;               // reset help-flag if not connected
		}

		//////////////////////////////
	}
}

extern void TCPClockHandler(void);

volatile uint32_t TimeTick = 0;
volatile uint32_t TimeTick2 = 0;


void taskTcpTick(void *param)
{
	const portTickType xDelay = 10 / portTICK_RATE_MS;

	while(1){

		TimeTick++;		// Increment first SysTick counter
		TimeTick2++;	// Increment second SysTick counter

		// After 100 ticks (100 x 10ms = 1sec)
		if (TimeTick >= 100) {
		  TimeTick = 0;	// Reset counter
		  LPC_GPIO1->FIOPIN ^= 1 << 25;	// Toggle user LED
		}
		// After 20 ticks (20 x 10ms = 1/5sec)
		if (TimeTick2 >= 20) {
		  TimeTick2 = 0; // Reset counter
		  TCPClockHandler();  // Call TCP handler
		}

		vTaskDelay( xDelay );
	}
}

