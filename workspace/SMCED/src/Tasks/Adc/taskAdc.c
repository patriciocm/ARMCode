/*
 * taskAdc.c
 *
 *  Created on: Oct 27, 2012
 *      Authorint16_t getAdcValue(ADC_type adc);: "Facundo Nahuel Uriel Silva"
 */

#include "taskAdc.h"

// Buffer to contain string version of value read from ADC
// 4 character, plus zero terminator
char strbuf [5];

uint32_t adcValueCount;
ADC_states adcState;

// Buffers de trabajo
medicion adcValues_buffer_A;
medicion adcValues_buffer_B;

medicion* adcCurrentBufferProcesing;
medicion* adcCurrentBufferWorking;

uint16_t adcFirtSincronizeValue;
uint16_t adcSecondSincronizeValue;

xSemaphoreHandle mutexAdcReady;

/////////////////////////////////////////////////////////////////////////////////

void ADC_IRQHandler(void){

	signed portBASE_TYPE pxHigherPriorityTaskWoken = pdFALSE;

	switch(adcState){

		case ADC_SINCRONIZE:

			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,DISABLE);
//			adcState = ADC_SINCRONIZING_DESC_WAIT_FIRST;

			adcFirtSincronizeValue = adcCurrentBufferWorking->values[adcValueCount].Adc0Value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
			adcValueCount = 0;

			LPC_GPIO2->FIOSET = (1<<0); // red
			LPC_GPIO2->FIOCLR = (1<<1); // green

			if( adcFirtSincronizeValue < UMBRAL_CERO ){
				adcState = ADC_SINCRONIZING_ASC_WAIT_FIRST;
			}

			break;

		case ADC_SINCRONIZING_ASC_WAIT_FIRST:

			adcSecondSincronizeValue = adcCurrentBufferWorking->values[adcValueCount].Adc0Value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);

			if( (adcSecondSincronizeValue <= UMBRAL_TECHO ) && (adcSecondSincronizeValue > (adcFirtSincronizeValue + UMBRAL_FLACO)) ){

				adcState = ADC_SINCRONIZING_ASC_WAIT_SECOND;
			}

			adcFirtSincronizeValue = adcSecondSincronizeValue;

			break;

		case ADC_SINCRONIZING_ASC_WAIT_SECOND:

			adcSecondSincronizeValue = adcCurrentBufferWorking->values[adcValueCount].Adc0Value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);

			if( (adcSecondSincronizeValue <= UMBRAL_TECHO ) && (adcSecondSincronizeValue > (adcFirtSincronizeValue + UMBRAL_FLACO)) ){
				LPC_GPIO2->FIOCLR = (1<<0); // red
				LPC_GPIO2->FIOSET = (1<<1); // green

				adcState = ADC_GET_ADC0_SAMPLE;
			}

			adcFirtSincronizeValue = adcSecondSincronizeValue;

			break;

		case ADC_START:
			adcState = ADC_GET_ADC0_SAMPLE;
			break;

		case ADC_GET_ADC0_SAMPLE:
			adcCurrentBufferWorking->values[adcValueCount].Adc0Value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_0);
			adcState = ADC_GET_ADC1_SAMPLE;
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,DISABLE);
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,ENABLE);

			// Verificaicon entrada
			if( (adcValueCount % MUESTRAS_POR_PERIODO) == MUESTRAS_CHECK_SINCRONISMO ){
				if(adcCurrentBufferWorking->values[adcValueCount].Adc0Value <= UMBRAL_RESINCRONISMO){
					adcState = ADC_SINCRONIZE;
				}
			}

			break;

		case ADC_GET_ADC1_SAMPLE:
			adcCurrentBufferWorking->values[adcValueCount].Adc1Value =  ADC_ChannelGetData(LPC_ADC,ADC_CHANNEL_1);
			adcState = ADC_GET_ADC0_SAMPLE;
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_1,DISABLE);
			ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);
			adcValueCount++;
			break;
	}

	if(adcValueCount == SIZE_SAMPLES ){

		// Se swapean los buffers
		if(adcCurrentBufferWorking == &adcValues_buffer_A){

			adcCurrentBufferWorking = &adcValues_buffer_B;
			adcCurrentBufferProcesing = &adcValues_buffer_A;

		}else{

			adcCurrentBufferWorking = &adcValues_buffer_A;
			adcCurrentBufferProcesing = &adcValues_buffer_B;
		}

		adcValueCount = 0;
		adcState = ADC_GET_ADC0_SAMPLE;

		// Se libera el mutex
		xSemaphoreGiveFromISR(mutexAdcReady,&pxHigherPriorityTaskWoken);

	}

	ADC_StartCmd(LPC_ADC,ADC_START_NOW);
	NVIC_EnableIRQ(ADC_IRQn);
}

/////////////////////////////////////////////////////////////////////////////////

void taskAdc(void *param){

	uint32_t i;
	uint32_t j;
	uint32_t counterCiclos = 0;

	float consumo = 0;
	float tension = 0;
	float corriente = 0;
	float cosPhi = 0 ;
	float frecuencia = 0;

	float meanTension = 0;
	float meanCorriente = 0;
	float meanCosPhi = 0;
	float meanFrecuencia = 0;

	uint32_t indexMaxV;
	uint32_t indexMaxI;

	uint32_t indexTensionCruzePorCero = 0;

	FIL fileMediciones;
	FIL fileSamples;

	FRESULT res;

	int8_t bufferWrite[150];
	int8_t sizeBufferWrite;
	uint32_t written;

	RTC_TIME_Type tiempo;

	PINSEL_CFG_Type PinCfg;

	adcCurrentBufferProcesing = &adcValues_buffer_A;
	adcCurrentBufferWorking = &adcValues_buffer_A;

	mutexAdcReady = xSemaphoreCreateMutex();
	xSemaphoreTake(mutexAdcReady, portMAX_DELAY);

	adcValueCount = 0;
	adcState = ADC_SINCRONIZE;
	////////////////

	PinCfg.Funcnum = 1;
	PinCfg.OpenDrain = 0;
	PinCfg.Pinmode = 0;
	PinCfg.Pinnum = 23;
	PinCfg.Portnum = 0;
	PINSEL_ConfigPin(&PinCfg);

	PinCfg.Pinnum = 24;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, 5000);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN0,ENABLE);
	ADC_IntConfig(LPC_ADC,ADC_ADINTEN1,ENABLE);

	/* preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(ADC_IRQn, ((0x01<<3)|0x01));

	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_0,ENABLE);

	// Start conversion
	ADC_StartCmd(LPC_ADC,ADC_START_NOW);

	/* Enable ADC in NVIC */
	NVIC_EnableIRQ(ADC_IRQn);

	///////////////

	sprintf(bufferWrite, HEADER_CSV_FILE);
	sizeBufferWrite = strlen(bufferWrite);

//	res = f_open(&fileMediciones, MEDICION_FILE, FA_CREATE_ALWAYS | FA_WRITE);
	while(f_open(&fileMediciones, MEDICION_FILE, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK);
	res = f_write(&fileMediciones, bufferWrite, sizeBufferWrite, &written);
	res = f_close(&fileMediciones);

	while(1){

		xSemaphoreTake(mutexAdcReady, portMAX_DELAY);

//		res = f_open(&fileSamples, LAST_SAMPLES_FILE, FA_CREATE_ALWAYS |FA_WRITE);

		meanTension = 0;
		meanCorriente = 0;
		meanCosPhi = 0;
		meanFrecuencia = 0;

		for(i=0; i < CANTIDAD_PERIODOS; i++){

			tension = 0;
			corriente = 0;
			indexMaxV = 0;
			indexMaxI = 0;
			indexTensionCruzePorCero = 0;

			for(j=0; j < MUESTRAS_POR_PERIODO; j++){

				float valueAdc0 = (float)( adcCurrentBufferProcesing->values[i*MUESTRAS_POR_PERIODO + j].Adc0Value - OFFSET_VALOR_TENSION );
				float valueAdc1 = (float)( adcCurrentBufferProcesing->values[i*MUESTRAS_POR_PERIODO + j].Adc1Value - OFFSET_VALOR_CORRIENTE );
				valueAdc1 *= -1;

				if(tension < valueAdc0){
					tension = valueAdc0;
					indexMaxV = j + 1;
				}

				if((indexMaxV != 0) && (indexTensionCruzePorCero == 0) && (valueAdc0 <= 0)){
					indexTensionCruzePorCero = j + 1;
				}

				if(corriente < valueAdc1){
					corriente = valueAdc1;
					indexMaxI = j + 1;
				}

//				sprintf(bufferWrite, "%.2f;%.2f;\n", valueAdc0, valueAdc1);
//				sizeBufferWrite = strlen(bufferWrite);
//				res = f_write(&fileSamples, bufferWrite, sizeBufferWrite, &written);

			}

			if(meanTension != 0)
				meanTension = (meanTension + tension*FACTOR_CONVERSION_TENSION)/2;
			else
				meanTension = tension*FACTOR_CONVERSION_TENSION;


			if(meanCorriente != 0)
				meanCorriente = (meanCorriente + corriente*FACTOR_CONVERSION_CORRIENTE)/2;
			else
				meanCorriente = corriente*FACTOR_CONVERSION_CORRIENTE;


			cosPhi = (float)indexMaxV;
			cosPhi -= (float)indexMaxI;
			cosPhi *= FACTOR_PHI;

			if(meanCosPhi != 0)
				meanCosPhi = (meanCosPhi + cos(cosPhi) )/2;
			else
				meanCosPhi = cos(cosPhi);


			frecuencia = (float)indexTensionCruzePorCero;
			frecuencia -= (float)(indexMaxV - 4);

			if(frecuencia > 0){
				if(meanFrecuencia != 0)
					meanFrecuencia = (meanFrecuencia + FACTOR_FRECUENCIA*frecuencia )/2;
				else
					meanFrecuencia = FACTOR_FRECUENCIA*frecuencia;
			}

		}

//		res = f_close(&fileSamples);

		if(meanCorriente <= 0.05){
			meanCorriente = 0;
			meanCosPhi = 0;
		}else{
			consumo += meanTension*meanCorriente*meanCosPhi*FACTOR_KILOWATT_HORA;
		}

		counterCiclos++;

		if(counterCiclos == CANTIDAD_CICLOS){


			RTC_GetFullTime(LPC_RTC,&tiempo);

			sprintf(bufferWrite, "%d/%d/%d %d:%d:%d;%.2f;%.2f;%.2f;%.2f;%.2f;\n",	tiempo.DOM,
																					tiempo.MONTH,
																					tiempo.YEAR,
																					tiempo.HOUR,
																					tiempo.MIN,
																					tiempo.SEC,
																					meanTension,
																					meanCorriente,
																					meanCosPhi,
																					meanFrecuencia,
																					consumo
			);

			sizeBufferWrite = strlen(bufferWrite);

			res = f_open(&fileMediciones, MEDICION_FILE, FA_WRITE);
			res = f_lseek(&fileMediciones,fileMediciones.fsize);
			res = f_write(&fileMediciones, bufferWrite, sizeBufferWrite, &written);
			res = f_close(&fileMediciones);


			counterCiclos = 0;
		}

	}

}

