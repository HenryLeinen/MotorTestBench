/*
 * menuManualControl.cpp
 *
 *  Created on: 20.06.2016
 *      Author: Henry
 */
#include <string.h>
#include "menuHandlers.h"
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "data.h"
#include "diag/trace.h"
#include <vector>

static bool bInitialized = false;
static uint32_t ticks_old = 0;		//	Used for measuring the time since the last change of gas was done by user.

std::vector<systemData_t>	Measurements;


static void menuCbkManualControlInit(void) {
	bMenuScreenDirty = true;
	bInitialized = true;
	Measurements.clear();
}

static void menuCbkManualControlDeinit(void) {
	bMenuScreenDirty = true;
	bInitialized = false;
}

bool menuCbkManualControl(bool btnDown, int16_t scrollTicks) {
	static bool btnDownOld = false;

	if (!bInitialized) {
		menuCbkManualControlInit();
	}

	//	First update the screen
	if (bMenuScreenDirty) {
		char buffer[40];
		uint16_t volt = (uint16_t)(systemData.voltage*100.0f), curr = (uint16_t)(systemData.current*100.f), powr = (uint16_t)(systemData.power*100.f);
		lcdPutsAt(0, 0, (char*)"Manual ");
		if (systemData.gas == 0)
			lcdPutsAt(7, 0, (char*)"Idle        ");
		else {
			sprintf(buffer, "M:%3d(%3d%%)", Measurements.size(), Measurements.size() == 0?0:Measurements[Measurements.size()-1].gas);
			lcdPutsAt(7, 0, buffer);
		}
		sprintf(buffer, "V= %2d.%02dV  ", volt/100, volt%100);
		lcdPutsAt(0, 1, buffer);
		sprintf(buffer, "I= %2d.%02dA", curr/100, curr%100);
		lcdPutsAt(11, 1, buffer);
		sprintf(buffer, "f= %5dHz  ", systemData.rpm);
		lcdPutsAt(0, 2, buffer);
		sprintf(buffer, "T= %5dg", (uint16_t)systemData.thrust);
		lcdPutsAt(11, 2, buffer);
		sprintf(buffer, "P=%3d.%02dW  ", powr/100, powr%100);
		lcdPutsAt(0, 3, buffer);
		sprintf(buffer, "ESC= % 3d%%", systemData.gas);
		lcdPutsAt(11, 3, buffer);
	}

	//	Adjust new ESC motor speed
	if (scrollTicks != 0) {
		systemData.gas += scrollTicks;
		if (systemData.gas < 0)
			systemData.gas = 0;
		if (systemData.gas > 100)
			systemData.gas = 100;
	}

	if (systemData.gas != 0) {
		static int16_t gas_old = 0;
		systemData.measure_offsets = false;

		//	Now if gas is != 0 record a new value every second after the gas value did not change
		if (gas_old != systemData.gas) {
			gas_old = systemData.gas;
			ticks_old = HAL_GetTick();
		} else {
			if (HAL_GetTick() - ticks_old > 1000000) {
				//	It is time to record a new value IF and ONLY IF
				//		- this is the first value to record OR
				//		- the previous value for GAS was different than the current
				if ((Measurements.size()==0) ||
					(Measurements[Measurements.size()-1].gas != systemData.gas)) {
					Measurements.push_back(systemData);
				}
				ticks_old = HAL_GetTick();
			}
		}
	} else
		systemData.measure_offsets = true;

	if (btnDown == true)
		btnDownOld = true;
	else
		if (btnDownOld) {
			btnDownOld = false;
			menuCbkManualControlDeinit();
			return true;
		}
	return false;
}
