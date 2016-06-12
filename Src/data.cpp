/*
 * data.c
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */
#include <stdint.h>
#include "data.h"
#include <stdlib.h>


systemData_t		systemData;
systemSettings_t	systemSettings;

void systemSettingsInit(void) {
	systemData.current = 0.0f;
	systemData.voltage = 0.0f;
	systemData.power	 = 0.0f;
	systemData.rpm     = 0;
	systemData.thrust	 = 0.0f;
	systemData.gas		= 0;

	systemData.sd_card_attached = false;
	systemData.battery_attached = false;
	systemData.usb_attached = false;


//	systemSettings.thrust_calibration_factor	= 445.0f;	// in digits per gramm [10kg weight sensor]
	systemSettings.thrust_calibration_factorN	= 5015;
	systemSettings.thrust_calibration_divisorN	= 1000;
	systemSettings.thrust_calibration_factor	= (float)systemSettings.thrust_calibration_factorN / (float)systemSettings.thrust_calibration_divisorN;	//	in digits per gramm [5kg weight sensor]
	systemSettings.display_contrast				= 25;
	systemSettings.thrust_offset				= 0;

	systemSettingsLoad();
}

void systemSettingsLoad(void) {

}

void systemSettingsSave(void) {

}
