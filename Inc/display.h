/*
 * display.h
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#include <stdint.h>

//	Prototype of the menu callback function.
//	This function will be called cyclically. The function must return true if the program is terminated and the control shall be given back
//	To the menu handler. Otherwise it shall return false.
//	The function will be provided with new user input as arguments
typedef bool (*menuCbkFct_t)(bool ButtonDown, int16_t scrollTicks);

typedef enum {
	MenuSubMenu,
	MenuProgram,
	MenuItemReturn,
	MenuItemEnd
} MenuItemType_t;

struct MenuEntry_t;

typedef struct MenuEntry_t {
	const char 		*MenuText;
	const char		*MenuDescription;
	MenuEntry_t*	SubMenuItems;
	MenuEntry_t*	ParentMenuItem;
	menuCbkFct_t	MenuCallback;
	MenuItemType_t	menuItemType;
} MenuEntry_t;

class displayMenu {
protected:
	MenuEntry_t*			m_pSelectedMenu;
	int16_t					m_nCurrentMenuItem;
	menuCbkFct_t			m_pActiveItem;

	int16_t					m_nMenuOffsetOnScreen;
	bool					m_bDirty;
public:
	//	Initializes the protected variable, initializes the menu struct by traversing through the linked list and fillingin the
	//	parent menu item information. Initializes the internal variables so that the initial menu item will be the very first menu item
	displayMenu(MenuEntry_t	*menuStruct);

	//	Initialize the LCD display
	void init();
	void processMenu(bool buttonPressState, int16_t scrollDir);

	void displaySetContrast(uint8_t);
protected:
	void drawStatus();
	void drawMenu();
};


#endif /* DISPLAY_H_ */
