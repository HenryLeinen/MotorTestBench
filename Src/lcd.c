//--------------------------------------------------------------
// File     : stm32_ub_lcd_2x16.c
// Datum    : 24.09.2015
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO
// Funktion : Text-LCD Funktionen (2x16 Zeichen)
//            im 4Bit-Mode
//            Chip : ST7066U/HD44780/SED1278/KS0066U/S6A0069X
// 
// Hinweis  : Das Display benutzt die CPU-Pins :
//             PE5  -> LCD_RS
//             PE6  -> LCD_E
//             PE7  -> LCD_DB4
//             PE8  -> LCD_DB5
//             PE9  -> LCD_DB6
//             PE10 -> LCD_DB7
//--------------------------------------------------------------

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include <lcd.h>
#include "mxconstants.h"

//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void lcdInitIo(void);
void _lcdSetPinLo(TLCD_NAME_t lcd_pin);
void _lcdSetPinHi(TLCD_NAME_t lcd_pin);
void _lcdClckPulse(void);
void lcdSendInitSequence(void);
void lcdCmd(uint8_t wert);
void lcdData(uint8_t wert);
void lcdDelay(volatile uint32_t nCount);
 


//--------------------------------------------------------------
// Definition aller Pins für das Display
// Reihenfolge wie bei TLCD_NAME_t
//
// Init : [Bit_SET,Bit_RESET]
//--------------------------------------------------------------
LCD_2X16_t LCD_2X16[] = {
 // Name   ,PORT 				, PIN       , Init
  {TLCD_RS ,LCD_RS_GPIO_Port	,LCD_RS_Pin	,GPIO_PIN_RESET},
  {TLCD_E  ,LCD_E_GPIO_Port		,LCD_E_Pin	,GPIO_PIN_RESET},
  {TLCD_D4 ,LCD_DB4_GPIO_Port	,LCD_DB4_Pin,GPIO_PIN_RESET},
  {TLCD_D5 ,LCD_DB5_GPIO_Port	,LCD_DB5_Pin,GPIO_PIN_RESET},
  {TLCD_D6 ,LCD_DB6_GPIO_Port	,LCD_DB6_Pin,GPIO_PIN_RESET},
  {TLCD_D7 ,LCD_DB7_GPIO_Port	,LCD_DB7_Pin,GPIO_PIN_RESET},
};



//--------------------------------------------------------------
// Init vom Text-LCDisplay
//--------------------------------------------------------------
void lcdInit(void)
{
  // init aller IO-Pins
  lcdInitIo();
  // kleine Pause
  lcdDelay(TLCD_INIT_PAUSE);
  // Init Sequenz starten
  lcdSendInitSequence();
  // LCD-Settings einstellen
  lcdCmd(TLCD_CMD_INIT_DISPLAY);
  lcdCmd(TLCD_CMD_ENTRY_MODE);
  // Display einschalten
  lcdCmd(TLCD_CMD_DISP_M1);
  // Display löschen
  lcdCmd(TLCD_CMD_CLEAR);
  // kleine Pause
  lcdDelay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// Löscht das Text-LCDisplay
//--------------------------------------------------------------
void lcdClear(void)
{
  // Display löschen
  lcdCmd(TLCD_CMD_CLEAR);
  // kleine Pause
  lcdDelay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// Stellt einen Display Mode ein
// mode : [TLCD_OFF, TLCD_ON, TLCD_CURSOR, TLCD_BLINK]
//--------------------------------------------------------------
void lcdSetMode(TLCD_MODE_t mode)
{
	switch (mode) {
	case TLCD_OFF:		lcdCmd(TLCD_CMD_DISP_M0); 	break;
	case TLCD_ON:		lcdCmd(TLCD_CMD_DISP_M1);	break;
	case TLCD_CURSOR:	lcdCmd(TLCD_CMD_DISP_M2);	break;
	case TLCD_BLINK:	lcdCmd(TLCD_CMD_DISP_M3);	break;
	}
}


//--------------------------------------------------------------
// Ausgabe von einem String auf dem Display an x,y Position
// x : 0 bis 15
// y : 0 bis 1
//--------------------------------------------------------------
void lcdPutsAt(uint8_t x, uint8_t y, char *ptr)
{
  // Cursor setzen
  lcdMoveCursor(x,y);
  // kompletten String ausgeben
  while (*ptr != 0) {
    lcdData(*ptr);
    ptr++;
  }
}

//--------------------------------------------------------------
// Speichern eines Sonderzeichens im CG-RAM vom Display
// nr : 0 bis 7 (nummer der sonderzeichens)
// pixeldata : 8 bytes mit Pixeldaten für das Zeichen
//--------------------------------------------------------------
void lcdPutsCG(uint8_t nr, uint8_t *pixeldata)
{
  uint8_t n;

  if(nr>7) nr=7;

  nr=(nr<<3);
  nr|=0x40;
  lcdCmd(nr);
  for(n=0;n<8;n++) {
    lcdData(pixeldata[n]);
  }
}

//--------------------------------------------------------------
// Ausgabe von einem Sonderzeichen auf dem Display an x,y Position
// x : 0 bis 15
// y : 0 bis 1
// nr : 0 bis 7 (nummer der sonderzeichens)
//--------------------------------------------------------------
void lcdPutsCGAt(uint8_t x, uint8_t y, uint8_t nr)
{
  if(nr>7) nr=7;

  // Cursor setzen
  lcdMoveCursor(x,y);
  lcdData(nr);
}

//--------------------------------------------------------------
// interne Funktion
// init aller IO-Pins
//--------------------------------------------------------------
void lcdInitIo(void)
{
  TLCD_NAME_t lcd_pin;
  
  for(lcd_pin=0;lcd_pin<TLCD_ANZ;lcd_pin++) {

    // Default Wert einstellen
    if(LCD_2X16[lcd_pin].TLCD_INIT==GPIO_PIN_RESET) {
      _lcdSetPinLo(lcd_pin);
    }
    else {
      _lcdSetPinHi(lcd_pin);
    }
  }  
}


//--------------------------------------------------------------
// interne Funktion
// Erzeuge neues Zeichen
//--------------------------------------------------------------
void lcdCreateCustomChar(uint8_t id, uint8_t data[8])
{
	uint8_t i;
	/* There are 8 custom character locations available */
	id &= 0x7;
	lcdCmd(0x40 | (id<<3));

	for(i = 0 ; i < 8 ; i++) {
		lcdData(data[i]);
	}
}


//--------------------------------------------------------------
// interne Funktion
// Pin auf Lo setzen
//--------------------------------------------------------------
void _lcdSetPinLo(TLCD_NAME_t lcd_pin)
{
	HAL_GPIO_WritePin(LCD_2X16[lcd_pin].TLCD_PORT, LCD_2X16[lcd_pin].TLCD_PIN, GPIO_PIN_RESET);
}


//--------------------------------------------------------------
// interne Funktion
// Pin auf Hi setzen
//--------------------------------------------------------------
void _lcdSetPinHi(TLCD_NAME_t lcd_pin)
{
	HAL_GPIO_WritePin(LCD_2X16[lcd_pin].TLCD_PORT, LCD_2X16[lcd_pin].TLCD_PIN, GPIO_PIN_SET);
}


//--------------------------------------------------------------
// interne Funktion
// einen Clock Impuls ausgeben
//--------------------------------------------------------------
void _lcdClckPulse(void)
{
  // Pin-E auf Hi
  _lcdSetPinHi(TLCD_E);
  // kleine Pause
  lcdDelay(TLCD_CLK_PAUSE);
  // Pin-E auf Lo
  _lcdSetPinLo(TLCD_E);
  // kleine Pause
  lcdDelay(TLCD_CLK_PAUSE);  
}


//--------------------------------------------------------------
// interne Funktion
// init Sequenz für das Display
//--------------------------------------------------------------
void lcdSendInitSequence(void)
{
  // Init Sequenz
  _lcdSetPinHi(TLCD_D4);
  _lcdSetPinHi(TLCD_D5);
  _lcdSetPinLo(TLCD_D6);
  _lcdSetPinLo(TLCD_D7);
  // Erster Init Impuls
  _lcdClckPulse();
  lcdDelay(TLCD_PAUSE);
  // Zweiter Init Impuls
  _lcdClckPulse();
  lcdDelay(TLCD_PAUSE);
  // Dritter Init Impuls
  _lcdClckPulse();
  lcdDelay(TLCD_PAUSE);
  // LCD-Modus einstellen (4Bit-Mode)
  _lcdSetPinLo(TLCD_D4);
  _lcdSetPinHi(TLCD_D5);
  _lcdSetPinLo(TLCD_D6);
  _lcdSetPinLo(TLCD_D7);
  _lcdClckPulse();
  lcdDelay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// interne Funktion
// Kommando an das Display senden
//--------------------------------------------------------------
void lcdCmd(uint8_t wert)
{
  // RS=Lo (Command)
  _lcdSetPinLo(TLCD_RS);
  // Hi-Nibble ausgeben         
  if((wert&0x80)!=0) _lcdSetPinHi(TLCD_D7); else _lcdSetPinLo(TLCD_D7);
  if((wert&0x40)!=0) _lcdSetPinHi(TLCD_D6); else _lcdSetPinLo(TLCD_D6);
  if((wert&0x20)!=0) _lcdSetPinHi(TLCD_D5); else _lcdSetPinLo(TLCD_D5);
  if((wert&0x10)!=0) _lcdSetPinHi(TLCD_D4); else _lcdSetPinLo(TLCD_D4);
  _lcdClckPulse();
  // Lo-Nibble ausgeben         
  if((wert&0x08)!=0) _lcdSetPinHi(TLCD_D7); else _lcdSetPinLo(TLCD_D7);
  if((wert&0x04)!=0) _lcdSetPinHi(TLCD_D6); else _lcdSetPinLo(TLCD_D6);
  if((wert&0x02)!=0) _lcdSetPinHi(TLCD_D5); else _lcdSetPinLo(TLCD_D5);
  if((wert&0x01)!=0) _lcdSetPinHi(TLCD_D4); else _lcdSetPinLo(TLCD_D4);
  _lcdClckPulse();  
}


//--------------------------------------------------------------
// interne Funktion
// Daten an das Display senden
//--------------------------------------------------------------
void lcdData(uint8_t wert)
{
  // RS=Hi (Data)
  _lcdSetPinHi(TLCD_RS);
  // Hi-Nibble ausgeben          
  if((wert&0x80)!=0) _lcdSetPinHi(TLCD_D7); else _lcdSetPinLo(TLCD_D7);
  if((wert&0x40)!=0) _lcdSetPinHi(TLCD_D6); else _lcdSetPinLo(TLCD_D6);
  if((wert&0x20)!=0) _lcdSetPinHi(TLCD_D5); else _lcdSetPinLo(TLCD_D5);
  if((wert&0x10)!=0) _lcdSetPinHi(TLCD_D4); else _lcdSetPinLo(TLCD_D4);
  _lcdClckPulse();
  // Lo-Nibble ausgeben        
  if((wert&0x08)!=0) _lcdSetPinHi(TLCD_D7); else _lcdSetPinLo(TLCD_D7);
  if((wert&0x04)!=0) _lcdSetPinHi(TLCD_D6); else _lcdSetPinLo(TLCD_D6);
  if((wert&0x02)!=0) _lcdSetPinHi(TLCD_D5); else _lcdSetPinLo(TLCD_D5);
  if((wert&0x01)!=0) _lcdSetPinHi(TLCD_D4); else _lcdSetPinLo(TLCD_D4);
  _lcdClckPulse();  
}


//--------------------------------------------------------------
// interne Funktion
// Cursor auf x,y stellen
//--------------------------------------------------------------
void lcdMoveCursor(uint8_t col, uint8_t row)
{
  uint8_t wert;
  uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};

  if(col>=TLCD_MAXX) col=0;
  if(row>=TLCD_MAXY) row=0;

  wert=col + rowOffsets[row];
  wert|=0x80;
  lcdCmd(wert);
}


//--------------------------------------------------------------
// kleine Pause (ohne Timer)
//--------------------------------------------------------------
void lcdDelay(volatile uint32_t nCount)
{
	HAL_Delay(nCount);
}

