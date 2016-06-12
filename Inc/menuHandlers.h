/*
 * menuHandlers.h
 *
 *  Created on: 02.06.2016
 *      Author: Henry
 */

#ifndef MENUHANDLERS_H_
#define MENUHANDLERS_H_

#include <stdint.h>
#include <string>
#include <vector>


/* Types used in file operation callbacks */
typedef enum
{
	Uninitialized,
	Waiting_for_SD_card,
	Reading_directory,
	Selecting_File,
	Display_error,
	Perform_operation,
	Done
} calibMenu_state_t;

typedef struct {
	char			name[20];
	uint32_t		size;
	bool			is_dir;
	bool			is_custom;
} dir_entry_info;

typedef bool (*pfnPerformAction)(uint16_t, bool, int16_t);
typedef bool (*pfnAddCustomItems)(uint8_t, const char** text);


/* Variables shared by all menu functions */
extern bool 		bMenuScreenDirty;
extern bool 		btnDownOld;

/* File operation internal functions */
extern bool menuCbkFileOperation(bool btnDown, int16_t scrollTicks, pfnPerformAction actionFunc, pfnAddCustomItems);

/* File operation menu functions */
extern bool menuCbkLoadCalibration(bool, int16_t);
extern bool menuCbkSaveCalibration(bool, int16_t);
extern bool menuCbkSaveData(bool, int16_t);
extern uint16_t		nFirstVisibleItem;
extern uint16_t		nSelectedDirItem;
extern std::string						calibCurrentDir;
extern std::string						calibErrorMsg;
extern std::vector<dir_entry_info>		dirItems;
extern calibMenu_state_t				_state;




extern bool menuCbkCalibWeight(bool, int16_t);

extern bool menuCbkCalibDisplay(bool, int16_t);

extern bool menuCbkCalibCurrent(bool, int16_t);

extern bool menuCbkCalibRPM(bool, int16_t);

extern bool menuCbkManualControl(bool, int16_t);

extern bool menuCbkAutoControl(bool, int16_t);

extern bool menuCbkESCEdit(bool, int16_t);


#endif /* MENUHANDLERS_H_ */
