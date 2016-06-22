/*
 * menuCailbWeight.cpp
 *
 *  Created on: 12.06.2016
 *      Author: Henry
 */
#include "menuHandlers.h"
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "data.h"
#include "diag/trace.h"





bool menuCbkCalibWeight(bool btnDown, int16_t scrollTicks) {
	char	buffer[20];
	//	Disable measurement of offset
	systemData.measure_offsets = false;

	if (scrollTicks > 0) {
		if (systemSettings.thrust_calibration_factorN < 1000000) {
			systemSettings.thrust_calibration_factorN+=scrollTicks;
			systemSettings.thrust_calibration_factor = (float)systemSettings.thrust_calibration_factorN/(float)systemSettings.thrust_calibration_divisorN;
			bMenuScreenDirty = true;
		}
	} else if (scrollTicks < 0) {
		if (systemSettings.thrust_calibration_factorN > -1000000) {
			systemSettings.thrust_calibration_factorN+= scrollTicks;
			systemSettings.thrust_calibration_factor = (float)systemSettings.thrust_calibration_factorN/(float)systemSettings.thrust_calibration_divisorN;
			bMenuScreenDirty = true;
		}
	}
	if (bMenuScreenDirty) {
		bMenuScreenDirty = false;
		sprintf(buffer, "%4.3f", systemSettings.thrust_calibration_factor);
		lcdClear();
		lcdPutsAt(0, 0, (char*)"Thrust coefficient:");
		lcdPutsAt(0, 1, buffer);
	}
	sprintf(buffer, "T=%5dg", (uint16_t)systemData.thrust);
	lcdPutsAt(0, 2, (char*)buffer);
	sprintf(buffer, "raw=%ld", (uint16_t)systemData.thrust_raw);
	lcdPutsAt(0, 3, (char*)buffer);
	if (btnDown == true)
		btnDownOld = true;
	else
		if (btnDownOld) {
			btnDownOld = false;
			systemData.measure_offsets = true;
			return true;
		}
	return false;
}
