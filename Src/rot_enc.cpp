/*
 * rot_enc.c
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */
#include "stm32f4xx_hal.h"
#include "rot_enc.h"
#include "tim.h"
#include "mxconstants.h"

/* ----- CONFIGURATION HINTS   -------- */
/*
 * within tim.h create the following two defines :
 * 	ROT_ENC_HANDLE --> must be the htim variable generated by MXCube for that particular channel
 * 	ROT_ENC_TIMER  --> must be the TIM device
 */

static uint16_t lastRotEncCounter = 0;

static void rotEncLowLevelInit(void) {
	ROT_ENC_INIT();
}


void rotEncInit(void) {
	/* Initialize the low level timer driver */
	rotEncLowLevelInit();

	/* Start the capture hardware */
	HAL_TIM_Encoder_Start_IT(&ROT_ENC_HANDLE, TIM_CHANNEL_ALL);
}

uint16_t rotEncGetPosition(void) {
	return 	ROT_ENC_TIMER->CNT;
}

int16_t rotEncGetScroll(void) {
	int16_t pos = rotEncGetPosition();
	int16_t	retval = (int16_t)pos - (int16_t)lastRotEncCounter;
	lastRotEncCounter = pos;
	return retval;
}

bool rotEncButtonPressed(void) {
	return HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin) == GPIO_PIN_SET;
}
