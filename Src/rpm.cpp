/*
 * rpm.cpp
 *
 *  Created on: 15.05.2016
 *      Author: Henry
 */

#include "stm32f4xx_hal.h"
#include "tim.h"
#include "rpm.h"
#include "diag/trace.h"
#include <numeric>

static uint32_t		rpmPeriods[16];


void rpmInit(void) {
	RPM_IN_INIT();

	HAL_TIM_IC_Start_DMA(&RPM_IN_HANDLE, TIM_CHANNEL_1, rpmPeriods, 16);
}

float rpmGetRPMValue(void) {
	//return zero value if invalid
	if (rpmPeriods[0] != 0)
		return 60000000.0f/(std::accumulate(rpmPeriods, rpmPeriods+16, 0.0f)/16.0f);
	return 0.0f;
}

float rpmGetRPSValue(void) {
	//	return zero value if invalid
	if (rpmPeriods[0] != 0)
		return 1000000.0f/(std::accumulate(rpmPeriods, rpmPeriods+16, 0.0f)/16.0f);
	return 0.0f;
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
	if (htim == &RPM_IN_HANDLE) {
//		trace_puts("Captured.");
	}
}
