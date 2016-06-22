/*
 * menuCalibDisplay.cpp
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






bool menuCbkCalibDisplay(bool btnDown, int16_t scrollTicks) {
	if (scrollTicks > 0) {
		if (systemSettings.display_contrast < 100) {
			systemSettings.display_contrast++;
			bMenuScreenDirty = true;
		}
	} else if (scrollTicks < 0) {
		if (systemSettings.display_contrast > 0) {
			systemSettings.display_contrast--;
			bMenuScreenDirty = true;
		}
	}
	if (bMenuScreenDirty) {
		bMenuScreenDirty = false;
		char	contrast[20];
		sprintf(contrast, "%3d %%", systemSettings.display_contrast);
		lcdClear();
		lcdPutsAt(0, 0, (char*)"Display Contrast:");
		lcdPutsAt(0, 1, contrast);
		lcdPutsAt(0, 2, (char*)"Press Button when");
		lcdPutsAt(0, 3, (char*)"ready !");
	}
	if (btnDown == true)
		btnDownOld = true;
	else
		if (btnDownOld) {
			btnDownOld = false;
			return true;
		}
	return false;
}
