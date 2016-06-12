/*
 * HX711.cpp
 *
 *  Created on: 06.05.2016
 *      Author: Henry
 */

#include "HX711.h"
#include "mxconstants.h"
#include "stm32f4xx_hal.h"
#include "diag/Trace.h"


HX711::~HX711() {
	// TODO Auto-generated destructor stub
}


void HX711::init() {
	GPIO_InitTypeDef  GPIO_InitStruct;

	__HAL_RCC_GPIOE_CLK_ENABLE();

	/* Configure the SCLK pin */
	GPIO_InitStruct.Pin = HX711_SCLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(HX711_SCLK_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(HX711_SCLK_GPIO_Port, HX711_SCLK_Pin, GPIO_PIN_RESET);

	/* Configure the DOUT pin */
	GPIO_InitStruct.Pin = HX711_DOUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(HX711_DOUT_GPIO_Port, &GPIO_InitStruct);
}

bool HX711::readSensor(float &val, HX711_Channel_t channel, bool bWait) {

	int32_t data = 0;
	/* Wait until the sensor is ready */
	if (bWait) {
		while (HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port, HX711_DOUT_Pin) == GPIO_PIN_SET)
			;
	} else {
		if (HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port, HX711_DOUT_Pin) == GPIO_PIN_SET)
			return false;
	}
	/* Pulse 25 times and read the value */
	for (uint8_t p = 0 ; p < (uint8_t)channel ; p ++) {
		HAL_GPIO_WritePin(HX711_SCLK_GPIO_Port, HX711_SCLK_Pin, GPIO_PIN_SET);
		/* Wait one microsecond */
		HAL_Delay (2);
		/* Read the data on the negative edge */
		HAL_GPIO_WritePin(HX711_SCLK_GPIO_Port, HX711_SCLK_Pin, GPIO_PIN_RESET);
		if (p < 24) {
			data |= (HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port, HX711_DOUT_Pin) == GPIO_PIN_SET) ? (1<<(31-p)) : 0;
		}
		/* Wait one microsecond */
		HAL_Delay (2);
	}

//	data = data ^0x80000000;
	val = (float)data/256.0f;
	return true;
}
