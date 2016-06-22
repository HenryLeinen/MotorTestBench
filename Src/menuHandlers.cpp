/*
 * menuHandlers.cpp
 *
 *  Created on: 02.06.2016
 *      Author: Henry
 */
#include "menuHandlers.h"
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "ff.h"
#include <string.h>
#include "data.h"

////////////////////////////////////////////////////////
//
//	Variables shared by all menu function
bool 		bMenuScreenDirty = true;
bool 		btnDownOld = false;



////////////////////////////////////////////////////////
//
//	menuLoadCalibration and menuSaveCalibration shared data
std::string	calibCurrentDir = "\\";
std::string	calibErrorMsg = "ok";
std::vector<dir_entry_info>		dirItems;
calibMenu_state_t	_state = Uninitialized;
uint16_t		nFirstVisibleItem = 0;
uint16_t		nSelectedDirItem = 0;


void menuFileOperationInit(void) {
	bMenuScreenDirty = true;
	btnDownOld = false;
	nFirstVisibleItem = 0;
	dirItems.clear();
	nSelectedDirItem = 0;
	_state = Waiting_for_SD_card;
}

void menuFileOperationDeinit(void) {
	bMenuScreenDirty = true;
	btnDownOld = false;
	nFirstVisibleItem = 0;
	nSelectedDirItem = 0;
	_state = Uninitialized;
}

void menuFileOperationLoadDirectory(pfnAddCustomItems customAdd) {
	FRESULT		fr;		/* return value */
	DIR			obj;	/* Directory search object */
	FILINFO		fno;	/* File information */

	//	Delete any previous items
	if (dirItems.size() != 0)
		dirItems.clear();

	//	Add the "return to menu" item
	dir_entry_info dei;
	strcpy(dei.name, "<-Return to Menu");
	dei.is_dir = false;
	dei.size = 0;
	dirItems.push_back(dei);

	//	Add a return to parent item, if we are not on the root dir
	fr = f_findfirst(&obj, &fno, calibCurrentDir.c_str(), "*");

	while(fr == FR_OK && fno.fname[0]) {
		dir_entry_info dei;
		strncpy(dei.name, fno.fname, 20);
		dei.size = fno.fsize;
		dei.is_dir = (fno.fattrib & AM_DIR) == AM_DIR;
		dei.is_custom = false;
		dirItems.push_back(dei);
		fr = f_findnext(&obj, &fno);
	}
	if (customAdd) {
		const char*	pText;
		uint8_t num = 0;
		while (customAdd(num++, &pText)) {
			strncpy(dei.name, pText, 20);
			dei.size = 0;
			dei.is_dir = 0;
			dei.is_custom = true;
			dirItems.push_back(dei);
		}
	}
	bMenuScreenDirty = true;
	nFirstVisibleItem = 0;
	nSelectedDirItem = 0;
}

void menuFileOperationShowDir(void) {

	if (bMenuScreenDirty) {
		lcdClear();
		for (uint8_t i = 0; i < 4 ; i++) {
			if (i + nFirstVisibleItem < dirItems.size()) {
				char szLine[25];
				if (nSelectedDirItem == i+nFirstVisibleItem)
					szLine[0] = '>';
				else
					szLine[0] = ' ';
				if (dirItems[i+nFirstVisibleItem].is_dir)
				{
					sprintf(&szLine[1], "(%s)", dirItems[i+nFirstVisibleItem].name);
				} else {
					sprintf(&szLine[1], "%s", dirItems[i+nFirstVisibleItem].name);
				}

				lcdPutsAt (0, i, szLine);
			}
		}
		bMenuScreenDirty = false;
	}
}

bool menuCbkFileOperation(bool btnDown, int16_t scrollTicks, pfnPerformAction actionFunc, pfnAddCustomItems addCustom) {


	switch (_state) {
	//	If uninitialized, initialize all data and proceed to next state
	case	Uninitialized:
		menuFileOperationInit();
		_state = Waiting_for_SD_card;
		break;

	//	As long as system does not report that an SD card is inserted, we will stay in this state
	//	A message will be displayed to the user prompting to insert the SD card.
	//	If the user has pressed a button, the function will return control to the parent menu.
	case	Waiting_for_SD_card:
		if (bMenuScreenDirty) {
			lcdClear();
			lcdPutsAt(0, 0, (char*)"Please insert ");
			lcdPutsAt(0, 1, (char*)"SD Card.");
			lcdPutsAt(0, 2, (char*)"Or press button");
			lcdPutsAt(0, 3, (char*)"to return.");
			bMenuScreenDirty = false;
		}

		if (!btnDown && btnDownOld) {
			menuFileOperationDeinit();
			return true;
		}

		if (systemData.sd_card_attached)
			_state = Reading_directory;
		break;

	//	Read Directory of
	case	Reading_directory:
		menuFileOperationLoadDirectory(addCustom);
		_state = Selecting_File;
		break;

	case	Selecting_File:
		//	Perform selection of the user
		if (!btnDown && btnDownOld) {
			//	User has made the selection
			//	Check if return to menu was selected
			if (strcmp(dirItems[nSelectedDirItem].name, "<-CANCEL")==0) {
				//	Will return control to the menu
				menuFileOperationDeinit();
				return true;
			} else if (dirItems[nSelectedDirItem].is_dir) {
				//	A directory entry has been selected, so change into that directory
				if (strcmp(dirItems[nSelectedDirItem].name, "..") == 0) {
					//	Change into the parent Dir
					size_t pos = calibCurrentDir.rfind("\\");
					if (pos != std::string::npos) {
						//	Found
						calibCurrentDir = calibCurrentDir.substr(0, pos);
						_state = Reading_directory;
						return false;
					}
				} else {
					//	Change into selected dir
					calibCurrentDir += "\\" + std::string(dirItems[nSelectedDirItem].name);
					_state = Reading_directory;
					return false;
				}
			} else {
				//	A file has been selected, perform the specific file operation
				_state = Perform_operation;
				bMenuScreenDirty = true;
				return false;
			}
		} else
		//	Move the selection according to user input
		if (scrollTicks > 0) {
			//	Move forward in current dir, but only if possible
			nSelectedDirItem ++;
			if (nSelectedDirItem >= dirItems.size())
				nSelectedDirItem--;
			else {
				if (nSelectedDirItem - nFirstVisibleItem > 3) {
					nFirstVisibleItem = nSelectedDirItem - 3;
				}
				bMenuScreenDirty = true;
			}
		} else if (scrollTicks < 0) {
			//	Move backwards in current dir, but only if possible
			if (nSelectedDirItem == 0)
				nSelectedDirItem =  0;
			else {
				nSelectedDirItem --;
				if (nSelectedDirItem < nFirstVisibleItem) {
					nFirstVisibleItem = nSelectedDirItem;
				}
				bMenuScreenDirty = true;
			}
		}
		menuFileOperationShowDir();
		break;

	case 	Display_error:
		if (bMenuScreenDirty) {
			lcdClear();
			lcdPutsAt(0, 0, (char *)calibErrorMsg.c_str());
		}
		if (!btnDown && btnDownOld) {
			menuFileOperationDeinit();
			return true;
		}
		break;

	case	Perform_operation:
		if (!actionFunc(nSelectedDirItem, btnDown, scrollTicks))		//	This function is responsible for cleaning up all internal variables as needed
			_state = Display_error;
		break;

	case	Done:
		menuFileOperationDeinit();
		return true;
		break;
	}

	// Process user input on button by taking over current state into the old state
	if (btnDown == true)
		btnDownOld = true;
	else
		if (btnDownOld) {
			btnDownOld = false;
		}
	return false;
}





bool menuCbkESCEdit(bool btnDown, int16_t scrollTicks) {
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


bool menuCbkSaveData(bool btnDown, int16_t scrollTicks) {
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






bool menuCbkCalibRPM(bool btnDown, int16_t scrollTicks) {
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

bool menuCbkAutoControl(bool btnDown, int16_t scrollTicks) {
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
