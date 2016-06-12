/*
 * menuLoadCalibration.cpp
 *
 *  Created on: 08.06.2016
 *      Author: Henry
 */
#include "menuHandlers.h"
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "data.h"
#include <string.h>
#include <stdlib.h>
#include "diag/trace.h"



static bool menuLoadCalibrationPerformAction(uint16_t nSelMenuItem, bool btnDown, int16_t scrollTicks) {
	char buffer[256];
	//	Open file and read file descriptor, if this is not the right file type, terminate with an error
	FRESULT	fr;
	FIL	f;

	//	Try open the file
	std::string path = calibCurrentDir + "\\" + dirItems[nSelMenuItem].name;
	fr = f_open(&f, path.c_str(), FA_READ);
	if (fr) {
		trace_printf("Error reading calibration file %s\n", path.c_str());
		calibErrorMsg = "Error load file";
		return false;
	}
	trace_printf("Reading calibration file %s\n", path.c_str());
	//	Now read as long as there is data
	while (f_gets(buffer, 256, &f) == buffer) {
		int i = strcspn(buffer, "=");
		char* arg = &buffer[i+1];
		buffer[i] = 0;
		if (strcmp(buffer, "Type") == 0) {
			if (strncmp(arg, "CALFILE1.0", 10) != 0) {
				calibErrorMsg = "Incorrect file !";
				return false;
			} else {
				trace_printf("File type match !\n");
			}
		} else if (strncmp(buffer, "thrust_offset", 13) == 0) {
			systemSettings.thrust_offset = atoi(arg);
			trace_printf("set thrust_offset to %ld\n", systemSettings.thrust_offset);
		} else if (strncmp(buffer, "thrust_calibration_factorN", 26) == 0) {
			systemSettings.thrust_calibration_factorN = atoi(arg);
			trace_printf("set thrust_calibration_factorN to %ld\n", systemSettings.thrust_calibration_factorN);
		} else if (strncmp(buffer, "thrust_calibration_divisorN", 27) == 0) {
			systemSettings.thrust_calibration_divisorN = atoi(arg);
			trace_printf("set thrust_calibration_divisorN to %ld\n", systemSettings.thrust_calibration_divisorN);
		} else if (strncmp(buffer, "display_contrast", 16) == 0) {
			systemSettings.display_contrast = atoi(arg);
			trace_printf("set display_contrast to %ld\n", systemSettings.display_contrast);
		} else {
			//	Overread !
		}
	}

	if (systemSettings.thrust_calibration_divisorN != 0)
		systemSettings.thrust_calibration_factor = (float)systemSettings.thrust_calibration_factorN / (float)systemSettings.thrust_calibration_factorN;
	f_close(&f);

	//	Adjust changed calibration

	_state = Done;

	return true;
}

bool menuCbkLoadCalibration(bool btnDown, int16_t scrollTicks) {

	return menuCbkFileOperation(btnDown, scrollTicks, menuLoadCalibrationPerformAction, NULL);
}
