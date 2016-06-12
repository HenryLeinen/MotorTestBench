/*
 * power_sense.c
 *
 *  Created on: 09.05.2016
 *      Author: Henry
 */

#include <stdlib.h>
#include <math.h>
#include "stm32f4xx_hal.h"
#include "power_sense.h"
#include "mxconstants.h"
#include "data.h"
#include "adc.h"
#include "diag/Trace.h"

#define	FILTER_AVERAGE	1
//#define FILTER_MEAN		1

#define CURRENT_INDEX 1
#define VOLTAGE_INDEX 0
#define NUM_MEASUREMENTS		50
#define FULL_RANGE_BITS			((float)((1<<12)-1))

volatile uint32_t adc_measured[2*NUM_MEASUREMENTS];
volatile uint32_t adc_offset;

void powerSenseInit(void) {
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_measured, 2*NUM_MEASUREMENTS);
}

float powerSenseGetCurrent(void) {
#ifdef FILTER_AVERAGE
	float madc_current = 0.0f;
	for (int i = 0 ; i < NUM_MEASUREMENTS ; i++) {
		madc_current+= adc_measured[2*i+CURRENT_INDEX];
	}
	madc_current = (madc_current / NUM_MEASUREMENTS) - (float)adc_offset;
#elif defined(FILTER_MEAN)
	static float madc_current = 0.0f;
	for (int i = 0 ; i < NUM_MEASUREMENTS ; i++) {
		madc_current = (3.0f*madc_current + adc_measured[2*i+CURRENT_INDEX])/4.0f;
	}
#endif
	return fabs( 3.3f*madc_current/FULL_RANGE_BITS / 0.133f);
}

float powerSenseGetVoltage(void) {
	float madc_voltage = 0.0f;
	for (int i = 0; i < NUM_MEASUREMENTS ; i++) {
		madc_voltage += adc_measured[2*i+VOLTAGE_INDEX];
	}
	madc_voltage /= NUM_MEASUREMENTS;
	return 7.818f * madc_voltage / FULL_RANGE_BITS*3.3;
}

void powerSenseGetNewOffset(void) {
	//	Take over the currently measured current and store it into the offset
	float madc_current = 0.0f;
	for (int i = 0 ; i < NUM_MEASUREMENTS ; i++) {
		madc_current+= adc_measured[2*i+CURRENT_INDEX];
	}
	adc_offset = (uint32_t)(madc_current / NUM_MEASUREMENTS);
}



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	(void)hadc;
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc == &hadc1) {
		trace_puts("Error in ADC conversion !");
		MX_ADC1_Init();
		HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_measured, 2*NUM_MEASUREMENTS);
	}
}
