/*
 * display.c
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */
#include <math.h>
#include "display.h"
#include "lcd.h"
#include "stm32f4xx_hal.h"
#include "dac.h"
#include "data.h"
#include <string.h>
#include "menuHandlers.h"
#include "diag/Trace.h"

extern MenuEntry_t FileMenuStruct[];
extern MenuEntry_t CalibMenuStruct[];
extern MenuEntry_t MainMenuStruct[];

MenuEntry_t		FileMenuStruct[] = {
	{ "LoadCal", "Load calibration data", 					NULL,				MainMenuStruct,	menuCbkLoadCalibration, MenuProgram	},
	{ "SaveCal", "Save calibration data",					NULL,				MainMenuStruct,	menuCbkSaveCalibration, MenuProgram	},
	{ "SaveData", "Save measured data",						NULL, 				MainMenuStruct,	menuCbkSaveData,		MenuProgram	},
	{ "Return",	"",											NULL,				MainMenuStruct,	NULL,					MenuItemReturn},
	{ "",			"",										NULL,				NULL,	NULL,					MenuItemEnd }
};

MenuEntry_t		CalibMenuStruct[] = {
	{ "Weight",	"Calibrate weight sensor",					NULL,				MainMenuStruct,	menuCbkCalibWeight,		MenuProgram	},
	{ "Display", "Calibrate display brightness",			NULL,				MainMenuStruct,	menuCbkCalibDisplay,	MenuProgram },
	{ "Current", "Calibrate current sensor",				NULL,				MainMenuStruct,	menuCbkCalibCurrent,	MenuProgram },
	{ "RPM",	"Calibrate RPM sensor",						NULL,				MainMenuStruct,	menuCbkCalibRPM,		MenuProgram },
	{ "Return",	"",											NULL,				MainMenuStruct,	NULL,					MenuItemReturn},
	{ "",			"",										NULL,				NULL,	NULL,					MenuItemEnd }
};

MenuEntry_t		MainMenuStruct[] = {
	{ "File", 		"Load or save calibration, Save data", 	FileMenuStruct, 	NULL,	NULL,					MenuSubMenu	},
	{ "Calibration","Calibrate display and sensors", 		CalibMenuStruct, 	NULL,	NULL,					MenuSubMenu	},
	{ "Manual",		"Manually control output",				NULL,				NULL, 	menuCbkManualControl,	MenuProgram	},
	{ "Sequencer",	"Automatic measurement program",		NULL,				NULL,	menuCbkAutoControl,		MenuProgram	},
	{ "ESC_Edit",	"Edit ESC properties",					NULL,				NULL,	menuCbkESCEdit,			MenuProgram	},
	{ "",			"",										NULL,				NULL,	NULL,					MenuItemEnd }
};






displayMenu::displayMenu(MenuEntry_t* pMenu)
	:	m_pSelectedMenu(pMenu), m_nCurrentMenuItem(0), m_pActiveItem(NULL), m_nMenuOffsetOnScreen(0), m_bDirty(true)
{
}

void displayMenu::init(void) {
	lcdInit();
	lcdClear();
	lcdSetMode(TLCD_ON);

	displaySetContrast(systemSettings.display_contrast);
}

void displayMenu::processMenu(bool buttonPressState, int16_t scrollDir) {
	static bool oldButtonPress = false;


	//	Check if currently a program is running
	if (m_pActiveItem != NULL) {
		if (m_pActiveItem(buttonPressState, scrollDir)) {
			//	Program terminated, so menu hanlding takes control again
			m_pActiveItem = NULL;
		}
	} else {
		//	draw Current sub menu in line no 1
		drawMenu();

		if (scrollDir > 0) {
			//	Check if we can scroll right
			if (m_pSelectedMenu[m_nCurrentMenuItem+1].menuItemType == MenuItemEnd) {
				//	It is not possible to scroll right, because menu is at the end already
			} else {
				//	Scroll right on step
				m_nCurrentMenuItem++;
				if ((m_nCurrentMenuItem-m_nMenuOffsetOnScreen) > 3)
					m_nMenuOffsetOnScreen++;
				m_bDirty = true;
			}
			oldButtonPress = false;
		} else if (scrollDir < 0) {
			//	Check if we can scroll left
			if (m_nCurrentMenuItem == 0) {
				//	It is not ppossible to scroll left, because menu is at the beginning already
			} else {
				//	Scroll left on step
				m_nCurrentMenuItem--;
				if ((m_nCurrentMenuItem - m_nMenuOffsetOnScreen) < 0)
					m_nMenuOffsetOnScreen--;
				m_bDirty = true;
			}
			oldButtonPress = false;
		} else if (buttonPressState ){
			oldButtonPress = true;
		} else if (!buttonPressState) {
			if (oldButtonPress) {
				//	Ok, now we detected a valid button press
				//	Navigate further in the menu
				switch( m_pSelectedMenu[m_nCurrentMenuItem].menuItemType) {
				case MenuSubMenu:
					m_pSelectedMenu = m_pSelectedMenu[m_nCurrentMenuItem].SubMenuItems;
					m_nCurrentMenuItem = 0;
					m_pActiveItem = NULL;
					m_bDirty = true;
					m_nMenuOffsetOnScreen = 0;
					break;
				case MenuProgram:
					m_pActiveItem = m_pSelectedMenu[m_nCurrentMenuItem].MenuCallback;
					m_bDirty = true;
					break;
				case MenuItemReturn:
					if (m_pSelectedMenu->ParentMenuItem != NULL)
						m_pSelectedMenu = m_pSelectedMenu->ParentMenuItem;
					m_nCurrentMenuItem = 0;
					m_pActiveItem = NULL;
					m_bDirty = true;
					break;
				default:
					break;
				}
				oldButtonPress = false;
			}
		}
	}
}


void displayMenu::displaySetContrast(uint8_t contrastInPercent) {
//	displayContrast = contrastInPercent;

	HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 4095 * contrastInPercent/100);
}

void displayMenu::drawStatus() {
	if (systemData.sd_card_attached) {
		lcdPutsAt(0, 0, (char*)" >SD< ");
	} else {
		lcdPutsAt(0, 0, (char*)"  SD  ");
	}
	if (systemData.usb_attached) {
		lcdPutsAt(5, 0, (char*)">USB< ");
	} else {
		lcdPutsAt(5, 0, (char*)" USB  ");
	}
	if (systemData.battery_attached) {
		lcdPutsAt(11, 0, (char*)">BAT< ");
	} else {
		lcdPutsAt(11, 0, (char*)" BAT  ");
	}
}


void displayMenu::drawMenu(void) {
	char MenuString[256] = {0};

	if (m_bDirty) {
		m_bDirty = false;
		lcdClear();

		int16_t i = m_nMenuOffsetOnScreen;
		int16_t r = 0;

		//	Check if the actual menu item
		while (m_pSelectedMenu[i].menuItemType != MenuItemEnd) {
			if (i == m_nCurrentMenuItem) {
				//	Draw selected
				strcpy (MenuString, "> ");
				strncat (MenuString, m_pSelectedMenu[i].MenuText, 18);
				lcdPutsAt(0, r++, MenuString);
			} else if ( r < 4) {
				//	Draw unselected
				strcpy (MenuString, "  ");
				strncat (MenuString, m_pSelectedMenu[i].MenuText, 18);
				lcdPutsAt(0, r++, MenuString);
			}
			i++;
		}
	} else {
		// Do nothing
	}
}

