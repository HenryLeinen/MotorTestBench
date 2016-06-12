/*
 * menuSaveCalibration.cpp
 *
 *  Created on: 12.06.2016
 *      Author: Henry
 */

#include "menuHandlers.h"
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "ff.h"
#include "data.h"
#include <string.h>
#include "diag/trace.h"



static char FileName[15] = "        ";
static uint8_t currentPos = 0;
static char *chars = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-";
static uint8_t curSelChar = 0;
static bool bEditModeActive = true;

static bool menuSaveCalibrationEditFileName(bool btnDown, int16_t scrollTicks) {
	if (!btnDown && btnDownOld) {
		//	User pressed key, now either enter or leave edit mode
		if (bEditModeActive) {
			bEditModeActive = false;
			lcdSetMode(TLCD_BLINK);
			if (currentPos < 7)
				currentPos++;
			else
				currentPos = 0;
			lcdMoveCursor(currentPos, 1);
			bMenuScreenDirty = false;
		} else {
			if (currentPos == 8) {
				//	We are finished !
				for (uint8_t i = 7; i != 0; i--)
					if (FileName[i] != ' ') {
						//	Append the extension
						FileName[++i] = '.';
						FileName[++i] = 'C';
						FileName[++i] = 'A';
						FileName[++i] = 'L';
						FileName[++i] = 0;
						return true;
					}
				return true;
			}
			lcdSetMode(TLCD_CURSOR);
			bEditModeActive = true;
			lcdMoveCursor(currentPos, 1);
			bMenuScreenDirty = false;
		}
	} else {
		if (bEditModeActive) {
			//	We are in editing mode
			if (scrollTicks > 0){
				curSelChar++;
				if (curSelChar == strlen(chars))
					curSelChar = 0;
				FileName[currentPos] = chars[curSelChar];
				bMenuScreenDirty = true;
			} else if (scrollTicks < 0) {
				if (curSelChar == 0)
					curSelChar = strlen(chars) -1;
				else
					curSelChar--;
				FileName[currentPos] = chars[curSelChar];
				bMenuScreenDirty = true;
			}
		} else {
			//	We are in non-editing mode, we can move the cursor
			if (scrollTicks > 0) {
				if (currentPos < 8) {
					currentPos++;
					//	Find this character
					bMenuScreenDirty = true;
				}
			} else if (scrollTicks < 0 ) {
				if (currentPos > 0) {
					currentPos--;
					bMenuScreenDirty = true;
				}
			}
			if (currentPos != 8) {
				uint8_t index = (uint8_t)(strchr(chars, FileName[currentPos]) - chars);
				curSelChar = index;
			}
		}
	}
	if (bMenuScreenDirty) {

		if (currentPos == 8) {
			lcdPutsAt(0, 2, (char*)"DONE? PRESS ENTER.");
			lcdSetMode(TLCD_ON);
		} else {
			lcdPutsAt(0, 2, (char*)"                  ");
			lcdPutsAt(currentPos,1,&FileName[currentPos]);
			if (!bEditModeActive)
				lcdSetMode(TLCD_BLINK);
			else
				lcdSetMode(TLCD_CURSOR);
			lcdMoveCursor(currentPos, 1);
		}
		bMenuScreenDirty = false;
	}
	return false;
}

static bool menuSaveCalibrationPerformAction(uint16_t nSelItem, bool btnDown, int16_t scrollTicks) {
	FIL	f;
	FRESULT fr;
	static bool bEditingFile = false;

	if (bEditingFile) {
		if (menuSaveCalibrationEditFileName(btnDown, scrollTicks)) {
			//	File editing is done, check if filename exists
			trace_printf("Creating new file %s\n", FileName);
			lcdClear();
			lcdPutsAt(0,0, (char*)"Saving calibration..");
			fr = f_open(&f, FileName, FA_CREATE_ALWAYS|FA_WRITE);
			if (fr) {
				calibErrorMsg = "Error create File";
				return false;
			}
			f_puts("Type=CALFILE1.0", &f);
			f_printf(&f, "thrust_offset=%ld\n", systemSettings.thrust_offset);
			f_printf(&f, "thrust_calibration_factorN=%ld\n", systemSettings.thrust_calibration_factorN);
			f_printf(&f, "thrust_calibration_divisorN=%ld\n", systemSettings.thrust_calibration_divisorN);
			f_printf(&f, "display_contrast=%ld\n", systemSettings.display_contrast);
			f_close(&f);
			_state = Done;
		}
	} else
	//	Check if the file new file or existing file
	if (dirItems[nSelItem].is_custom) {
		if (strcmp(dirItems[nSelItem].name, "<NEW FILE>") == 0) {
			//	Create a new file and store data in new file
			trace_puts("Creating new file for calibration data !");
			bEditingFile = true;
			lcdClear();
			lcdPutsAt(0,0,"Enter Filename :");
			lcdPutsAt(0,1,FileName);
			bMenuScreenDirty = true;
		} else {
			//	This should never happen, we have no other custom actions here
			calibErrorMsg = "Internal ERROR!";
			return false;
		}
	} else {
		//	File exists, so we just open it to overwrite it
		trace_puts("Opening existing file for writing calibration data !");
		std::string path = calibCurrentDir + "\\" + dirItems[nSelItem].name;
		fr = f_open(&f, path.c_str(), FA_CREATE_ALWAYS|FA_WRITE);
		if (fr) {
			calibErrorMsg = "Error create File";
			return false;
		}
		f_puts("Type=CALFILE1.0\n", &f);
		f_printf(&f, "thrust_offset=%ld\n", systemSettings.thrust_offset);
		f_printf(&f, "thrust_calibration_factorN=%ld\n", systemSettings.thrust_calibration_factorN);
		f_printf(&f, "thrust_calibration_divisorN=%ld\n", systemSettings.thrust_calibration_divisorN);
		f_printf(&f, "display_contrast=%ld\n", systemSettings.display_contrast);
		f_close(&f);
		_state = Done;
	}
	return true;
}


static bool menuSaveCalibrationAddCustomItems(uint8_t itemno, const char** itemText) {
	if (itemno == 0) {
		*itemText = "<NEW FILE>";
		return true;
	}
	return false;
}

bool menuCbkSaveCalibration(bool btnDown, int16_t scrollTicks) {

	return menuCbkFileOperation(btnDown, scrollTicks, menuSaveCalibrationPerformAction, menuSaveCalibrationAddCustomItems);
}



