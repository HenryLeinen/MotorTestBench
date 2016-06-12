/*
 * esc.c
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */
#include "stm32f4xx_hal.h"
#include "esc.h"
#include "tim.h"





static void escLowLevelInit(void) {
	ESC_CTRL_INIT();
}

void escInit(void) {

	//	Init the low level hardware first
	escLowLevelInit();

	//	Calibrate the ESC
	//	Set the output to max when activating the PWM
	escSetOutputRaw(2000);

	//	Wait 1 second and switch output to idle
	HAL_Delay(1000000);
	escSetOutput(0);
}

void escSetOutputRaw(uint16_t raw) {
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = raw;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&ESC_CTRL_HANDLE, &sConfigOC, ESC_CONTROL_TIMER_CHANNEL);
	HAL_TIM_PWM_Start(&ESC_CTRL_HANDLE, ESC_CONTROL_TIMER_CHANNEL);
}

void escSetOutput(uint8_t percent) {
	escSetOutputRaw(1000 + percent*10);
}

