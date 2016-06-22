/*
 * menuCalibCurrent.cpp
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



bool menuCbkCalibCurrent(bool btnDown, int16_t scrollTicks) {
	static bool btnDownOld = false;
	lcdPutsAt(3, 0, (char*)"ESC Edit Function   ");
	if (btnDown == true)
		btnDownOld = true;
	else
		if (btnDownOld) {
			btnDownOld = false;
			return true;
		}
	return false;
}
