/*
===============================================================================
 Name        : main.c
 Author      :
 Version     :
 Copyright   : Copyright (C)
 Description : main definition
===============================================================================
*/
#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include <NXP/crp.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>

// Variable to store CRP value in. Will be placed automatically
// by the linker when "Enable Code Read Protect" selected.
// See crp.h header for more information
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

// TODO: insert other include files here
#include "debug.h"

#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "auxiliares.h"
#include "rtc.h"


// TODO: insert other definitions and declarations here
#define SERIAL 1 //MODO de Operacion
#define DEBUG_1 0

//Vectores de Almacenamiento de Muestras
volatile uint32_t VectorTiempoMain[MAX_TAMANIO*2];
volatile uint16_t VectorMedicionesFinal[MAX_TAMANIO/2];
volatile uint32_t VectorTiempoFinal[MAX_TAMANIO/2];

//Contador de interrupciones del timer0
extern volatile uint32_t timer0_m0_counter;




int main(void)
{
	//Definiciones utilizadas por el ADC
	float SUPPLY_VOLTAGE = 3.3;    //en volts
	double ADC_COUNT_MAX = 4095;
	float ADC_STEP = SUPPLY_VOLTAGE/ADC_COUNT_MAX;
	float GAP_VOLTAGE = 0.045; //en volts
	int GAP_COUNT = GAP_VOLTAGE/ADC_STEP;
	float CIENMIL = 100000;
	int MIL = 10000;
	float CONSTANTE_TIMER = 1136/CIENMIL;


#ifdef SERIAL
		UARTInit(3, 9600);	/* baud rate setting */
		UARTInit(1, 9600);
		uint8_t tramaATransmitir[200];

		tramaATransmitir[0] = 0x7E;
		tramaATransmitir[1] = 0x00;
		tramaATransmitir[2] = 0x07;
		tramaATransmitir[3] = 0x01;
		tramaATransmitir[4] = 0x01;
		tramaATransmitir[5] = 0x00;
		tramaATransmitir[6] = 0x01;
		tramaATransmitir[7] = 0x00;
		tramaATransmitir[8] = 0x12;

		uint8_t checksumAux = 0x15;
#endif

	//Resetear Valores de Calculo de Volumen
	resetMediciones();

	//Update variable SystemCoreClock
	SystemCoreClockUpdate();

	float prueba = 0.10;
	float resultadoPrueba = logf(prueba);

	if(DEBUG_1)
	{		debug_printf("PRUEBA LOGARITMO:%d.%04d\n", (int) resultadoPrueba, (int)((resultadoPrueba-(int)resultadoPrueba)*10000));
			debug_printf("CPU Clock:%dMHz\n", SystemCoreClock/1000000);
			debug_printf("ADC STEP:%d.%04dV\n", (int)ADC_STEP, (int)((ADC_STEP-(int)ADC_STEP)*10000));
			debug_printf("GAP VOLTAGE:%d.%03dV\n", (int)GAP_VOLTAGE, (int)((GAP_VOLTAGE-(int)GAP_VOLTAGE)*1000));
			debug_printf("GAP COUNT:%d\n", (int) GAP_COUNT);
			debug_printf_flush();
	}


	//Initialize ADC
	ADCInit(ADC_CLK);

	//Initialize Timer0
	init_timer( 0 );
	enable_timer(0);

	//Calculo de valor de tension media utilizado para comparar si se guardan futuros valores medidos
	uint32_t sumatoria = 0;
	uint32_t cantidadMuestras = 0;
	uint32_t i;

	//Start ADC
	ADCBurstRead();

	while (cantidadMuestras < MAX_TAMANIO*3 )
	{
		while (!vectorReady)
		{}

		if(vectorReady == 1)
		{
			for( i=0 ; i< MAX_TAMANIO/2; i++)
			{	sumatoria += VectorMedicionesMain[i];
			}
		}
		else
		{
			for( i=MAX_TAMANIO/2 ; i < MAX_TAMANIO; i++)
			{	sumatoria += VectorMedicionesMain[i];
			}
		}
		vectorReady = 0;
		cantidadMuestras+=MAX_TAMANIO/2;
	}

	//Stop ADC
	LPC_ADC->ADCR &= ~(0x01 << 16);

	uint32_t promedioCuentas = sumatoria/cantidadMuestras;
	double promedioTension = promedioCuentas*ADC_STEP;

	for( i=0 ; i< MAX_TAMANIO; i++)
	{	VectorMedicionesMain[i] = 0;
	}

	if(DEBUG_1)
	{		debug_printf("Valor promedio (V): %d.%03dV\n", (int)promedioTension, (int)((promedioTension-(int)promedioTension)*1000));
    		debug_printf("Valor promedio (Cuentas): %d\n", (int)promedioCuentas);
	}

    //Inicializar vector de MedicionesMain
    VectorMedicionesMain[0] = promedioCuentas;
    VectorTiempoMain[0] = 0;

    //uint8_t count = 0;
    uint8_t imprimir = 1;
	int32_t lastMain = 0;
	int32_t lastFinal = 0;
	int32_t tiempoInicial = 0;
	int16_t maxIndice;

	uint16_t auxMuestra[4];
	uint32_t auxTiempo[4];
	uint32_t difference = 0;
	uint8_t largoSenial = 0;
	uint32_t duracionSenial = 0;
	uint32_t moduloSenial = 0;
	uint32_t moduloCuadradoSenial = 0;
	double tensionMedida = 0;
	double tiempoMedido = 0;

	uint32_t primeraCondicion=0;
	uint32_t segundaCondicion=0;
	uint32_t terceraCondicion=0;

	//Reset SystickTimer
	/*
	SysTick->CTRL  &= ~(0x01 << 0);
	SysTick->VAL   = 0;
	SysTick->CTRL  |= (0x01 << 0);
	*/

	goodmeasures = 0;
    timer0_m0_counter = 0;

    vectorActivo = 3;
    vectorReady = 0;

    /* Initialize RTC module */
    RTCInit();
    RTCStart();

	//Start ADC
    ADCBurstRead();

    //uint16_t stupidCompiler = MAX_TAMANIO/2;
    uint16_t stupidCompiler = 50;

	while(1)
	{
		 while(!vectorReady)
		 {}

		 vectorReady = 0;

		 if(alarm_on)
		 {
			 alarm_on = 0;
			 enviarDatos();
		 }

		 if(lastFinal < stupidCompiler)
		 {
			 if(VectorMedicionesMain[0] > ADC0Value[0])
				 difference = VectorMedicionesMain[0] - ADC0Value[0];
			 else
				 difference = ADC0Value[0] - VectorMedicionesMain[0];

			 if(difference > GAP_COUNT)
			 {
				 switch (largoSenial)
				 {
				 	 case 0x00:
						 auxMuestra[0] = ADC0Value[0];
						 auxTiempo[0] = timer0_m0_counter;
						 largoSenial = 1;
						 break;
				 	 case 0x01:
				 		 if( (timer0_m0_counter - auxTiempo[0]) < 10 )
				 		 {	 auxTiempo[1] = timer0_m0_counter;
				 			 auxMuestra[1] = ADC0Value[0];
				 		 	 largoSenial = 2;
				 		 }
				 		 else
				 		 {	 auxTiempo[0] = timer0_m0_counter;
				 			 auxMuestra[0] = ADC0Value[0];
						 	 largoSenial = 1;
				 		 }
				 		 break;
				 	 case 0x02:
						 if( (timer0_m0_counter - auxTiempo[0]) < 10 )
						 {	 auxTiempo[2] = timer0_m0_counter;
							 auxMuestra[2] = ADC0Value[0];
							 largoSenial = 3;
						 }
						 else
						 {	 auxTiempo[0] = timer0_m0_counter;
							 auxMuestra[0] = ADC0Value[0];
							 largoSenial = 1;
						 }
						 break;
				 	 case 0x03:
				 		 if( (timer0_m0_counter - auxTiempo[1]) < 10 )
						 {
				 			 auxTiempo[3] = timer0_m0_counter;
			 			 	 auxMuestra[3] = ADC0Value[0];

			 			 	 maxIndice = 4;
				 			 for( i=4 ; i>0; i--)
							 {	VectorTiempoMain[i] = auxTiempo[(i-1)];
				 				VectorMedicionesMain[i] = auxMuestra[(i-1)];
				 				//Calculo de | | total de la senial
				 				//aux = VectorMedicionesMain[i]*ADC_STEP;
				 				//moduloSenial += aux;
				 				moduloSenial += VectorMedicionesMain[i];
				 				//Calculo de | |^2 total de la senial
				 				moduloCuadradoSenial += VectorMedicionesMain[i]*VectorMedicionesMain[i];
				 				//moduloCuadradoSenial += pow( VectorMedicionesMain[i]*ADC_STEP, 2);
				 				if( VectorMedicionesMain[i] > VectorMedicionesMain[maxIndice])
				 					 maxIndice = i;
							 }

				 			 lastMain = 4;
				 			 goodmeasures = 0;

				 			 //CALCULO DE LA DURACION DE LA GOTA
				 			 int16_t condicionCorte = 1;
				 			 int16_t firsttime = 1;
				 			 int32_t lastValidSample = timer0_m0_counter;

				 			 SysTick->CTRL &= ~( 0x01 << 1);
				 			 tiempoInicial = timer0_m0_counter;
				 			 while (condicionCorte)
				 			 { 	  while(!vectorReady )
		 				 	 	  {}
		 				 	 	  vectorReady = 0;
				 				  if(ADC0Value[0] < GAP_COUNT && timer0_m0_counter > (lastValidSample + 1000) )
							 	  {		condicionCorte = 0;
							 	  }
				 				  else
				 				  {		if( ADC0Value[0] > GAP_COUNT )
				 				  	  	{	lastValidSample = timer0_m0_counter;
				 				  	  	}
										lastMain++;
										VectorTiempoMain[lastMain] = timer0_m0_counter;
										VectorMedicionesMain[lastMain] = ADC0Value[0];
										//Calculo de | | total de la senial
										moduloSenial += VectorMedicionesMain[lastMain];
										//Calculo de | |^2 total de la senial
										moduloCuadradoSenial += VectorMedicionesMain[lastMain]*VectorMedicionesMain[lastMain];
										if( moduloCuadradoSenial > 100000 )
										{	 moduloCuadradoSenial = moduloCuadradoSenial/MIL;
											 firsttime = 0;
										}
										if( VectorMedicionesMain[lastMain] > VectorMedicionesMain[maxIndice])
										{	maxIndice = lastMain;
										}
										if(lastMain > 3000 )
									    {	lastMain = 3000;
									    }
							 	  }
							 }
				 			 duracionSenial = timer0_m0_counter - tiempoInicial;
				 			 SysTick->CTRL |= (0x01 << 1);

				 			 //float auxinicial = timer0_m0_counter;
				 			 calcularVolumen(VectorMedicionesMain[maxIndice]*ADC_STEP, moduloSenial, duracionSenial);
				 			 //float auxfinal = timer0_m0_counter - auxinicial;*/
				 			 //debug_printf("Tiempo de Calculo Volumen: %d\n", (int)auxfinal);

							 //Cargar en VectorMedicionesFinal y VectorTiempoFinal el pico +- 10 muestras
							 if(maxIndice < 5)
							 {
								 for( i=1 ; i < (maxIndice+6) ; i++)
								 {	 VectorTiempoFinal[lastFinal] = VectorTiempoMain[i];
									 VectorMedicionesFinal[lastFinal] = VectorMedicionesMain[i];
									 lastFinal++;
								 }
								 primeraCondicion++;
							 }
							 else if (maxIndice > (lastMain-6) )
							 {
								 for( i=maxIndice-5 ; i < lastMain ; i++)
								 {	 VectorTiempoFinal[lastFinal] = VectorTiempoMain[i];
									 VectorMedicionesFinal[lastFinal] = VectorMedicionesMain[i];
									 lastFinal++;
								 }
								 segundaCondicion++;
							 }
							 else
							 {   for( i=maxIndice-4 ; i < (maxIndice+6) ; i++)
								 {	 VectorTiempoFinal[lastFinal] = VectorTiempoMain[i];
									 VectorMedicionesFinal[lastFinal] = VectorMedicionesMain[i];
									 lastFinal++;
								 }
								 terceraCondicion++;
							 }

							 if(SERIAL)
							 {
								 uint16_t j;

								 //Transmitir los valores de los vectores VectorTiempoMain y VectorMedicionesMain
								 //por puerto serial
								 //Cargando 32bits de Duracion de la Senial
								 tramaATransmitir[9]= duracionSenial >> 24;
								 tramaATransmitir[10]= duracionSenial >> 16;
								 tramaATransmitir[11]= duracionSenial >> 8;
								 tramaATransmitir[12]= duracionSenial;
								 //Cargando 32bits de Suma de Modulos
								 tramaATransmitir[13]= moduloSenial >> 24;
								 tramaATransmitir[14]= moduloSenial >> 16;
								 tramaATransmitir[15]= moduloSenial >> 8;
								 tramaATransmitir[16]= moduloSenial;
								 //Cargando 32bits de Suma de Modulos Cuadrados
								 tramaATransmitir[17]= moduloCuadradoSenial >> 24;
								 tramaATransmitir[18]= moduloCuadradoSenial >> 16;
								 tramaATransmitir[19]= moduloCuadradoSenial >> 8;
								 tramaATransmitir[20]= moduloCuadradoSenial;

								 for(j=9; j < 21; j++)
								 {
								 	checksumAux += tramaATransmitir[j];
								 }

								 for(i=0; i < lastFinal ; i++)
								 {
								 	//Cargando 32bits de Tiempo de la Muestra
								 	 tramaATransmitir[21+6*i]= VectorTiempoFinal[i] >> 24;
									 tramaATransmitir[22+6*i]= VectorTiempoFinal[i] >> 16;
									 tramaATransmitir[23+6*i]= VectorTiempoFinal[i] >> 8;
									 tramaATransmitir[24+6*i]= VectorTiempoFinal[i];
								 	 //Cargando 16bits Valor de la Muestra
								 	 tramaATransmitir[25+6*i]= VectorMedicionesFinal[i] >> 8;
								 	 tramaATransmitir[26+6*i]= VectorMedicionesFinal[i];

								 	//checksum
								 	for(j=21; j < 27; j++)
								 	{
								 		checksumAux += tramaATransmitir[j+6*i];
								 	}

								 }

								 tramaATransmitir[2]= (6 + 4 + 4 + 4 ) + lastFinal*6;
								 tramaATransmitir[3 + tramaATransmitir[2]]= 0xFF - checksumAux;
								 UARTSend(3, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
								 UARTSend(1, (uint8_t *)tramaATransmitir , 4 + tramaATransmitir[2] );
								 //delayFunction(13);   //Espero 4ms
								 //debug_printf("Se transmitio trama %d\n", count);
								 //count++;
								 lastFinal = 0;
								 checksumAux = 0x15;

							 }
							 else
							 {	int32_t auxiliarOverrun = overrun;
							 	int32_t auxiliarGoodmeasures = goodmeasures;
								for( i=0 ; i <= lastMain; i++)
							 	{
									//tensionMedida = VectorMedicionesMain[i]*ADC_STEP;
							 	 	 //tiempoMedido = VectorTiempoMain[i]*CONSTANTE_TIMER;
							 	 	 tensionMedida = VectorMedicionesMain[i];
							 	 	 tiempoMedido = VectorTiempoMain[i];
							 	 	 debug_printf("%d.%03d ", (int)tiempoMedido, (int)((tiempoMedido-(int)tiempoMedido)*1000));
							 	 	 debug_printf("%d.%03d\n", (int)tensionMedida, (int)((tensionMedida-(int)tensionMedida)*1000));
							 	 	 debug_printf_flush();

							 	}

								if(DEBUG_1)
								{	debug_printf("Valor Modulo: %d.%03d\n", (int)moduloSenial, (int)((moduloSenial-(int)moduloSenial)*1000));
							 		debug_printf("Valor Modulo al Cuadrado: %d.%03d\n", (int)moduloCuadradoSenial, (int)((moduloCuadradoSenial-(int)moduloCuadradoSenial)*1000));
							 		debug_printf("Cantidad de GoodMeasures: %d\n", auxiliarGoodmeasures);
							 		debug_printf("Statistics: Cantidad de Overruns: %d . ", auxiliarOverrun);
							 		debug_printf_flush();
								}
							 }

				 			 largoSenial = 0;
				 			 lastMain = 0;
				 			 moduloSenial = 0;
				 			 moduloCuadradoSenial = 0;
						 }
						 else
						 {	 auxTiempo[0] = timer0_m0_counter;
							 auxMuestra[0] = ADC0Value[0];
							 largoSenial = 1;
						 }
						 break;
				 	 default:
				 		 break;
				 }
			 }
		 }
		 else
		 {
			 LPC_ADC->ADCR &= ~(0xFF << 16);
			 if(imprimir)
			 {
				 if(DEBUG_1)
				 {	 debug_printf("Statistics: Cantidad de Overruns: %d . ", overrun);
				 	 debug_printf("Cantidad de GoodMeasures: %d\n", goodmeasures);
				 	 debug_printf("Primera Condicion: %d\n", primeraCondicion);
				 	 debug_printf("Segunda Condicion: %d\n", segundaCondicion);
				 	 debug_printf("Tercera Condicion: %d\n", terceraCondicion);

				 	 for( i=0 ; i < stupidCompiler+10; i++)
				 	 {	 tensionMedida = VectorMedicionesFinal[i]*ADC_STEP;
				 	 	 tiempoMedido = VectorTiempoFinal[i]*CONSTANTE_TIMER;
				 	 	 debug_printf("%d.%03d ", (int)tiempoMedido, (int)((tiempoMedido-(int)tiempoMedido)*1000));
				 	 	 debug_printf("%d.%03d\n", (int)tensionMedida, (int)((tensionMedida-(int)tensionMedida)*1000));
				 	 }
				 	 imprimir = 0;
				 }
			 }
		 }
	}
	return 0;
}











