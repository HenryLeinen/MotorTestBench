//--------------------------------------------------------------
// File     : stm32_ub_lcd_2x16.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_LCD_2X16_H
#define __STM32F4_UB_LCD_2X16_H

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx_hal.h"
#include <stdint.h>

//--------------------------------------------------------------
// Display Modes
//--------------------------------------------------------------
typedef enum {
  TLCD_OFF =0,    // Display=AUS, Cursor=Aus, Blinken=Aus
  TLCD_ON,        // Display=EIN, Cursor=Aus, Blinken=Aus
  TLCD_CURSOR,    // Display=EIN, Cursor=EIN, Blinken=Aus
  TLCD_BLINK      // Display=EIN, Cursor=EIN, Blinken=EIN
}TLCD_MODE_t;


//--------------------------------------------------------------
// Defines
//--------------------------------------------------------------
#define  TLCD_INIT_PAUSE    100000  // pause beim init 40ms
#define  TLCD_PAUSE          2000  // kleine Pause (>=20000)
#define  TLCD_CLK_PAUSE       100  // pause f�r Clock-Impuls (>=500)
#define  TLCD_MAXX             40  // max x-Position (0...15)
#define  TLCD_MAXY              4  // max y-Position (0...1)


//--------------------------------------------------------------
// LCD Kommandos (siehe Datenblatt)
//--------------------------------------------------------------
#define  TLCD_CMD_INIT_DISPLAY  0x28   // 2 Zeilen Display, 5x7 Punkte
#define  TLCD_CMD_ENTRY_MODE    0x06   // Cursor increase, Display fix
#define  TLCD_CMD_DISP_M0       0x08   // Display=AUS, Cursor=Aus, Blinken=Aus
#define  TLCD_CMD_DISP_M1       0x0C   // Display=EIN, Cursor=AUS, Blinken=Aus
#define  TLCD_CMD_DISP_M2       0x0E   // Display=EIN, Cursor=EIN, Blinken=Aus
#define  TLCD_CMD_DISP_M3       0x0F   // Display=EIN, Cursor=EIN, Blinken=EIN 
#define  TLCD_CMD_CLEAR         0x01   // loescht das Display



//--------------------------------------------------------------
// Liste aller Pins f�r das Display
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum 
{
  TLCD_RS = 0,  // RS-Pin
  TLCD_E  = 1,  // E-Pin
  TLCD_D4 = 2,  // DB4-Pin
  TLCD_D5 = 3,  // DB5-Pin
  TLCD_D6 = 4,  // DB6-Pin
  TLCD_D7 = 5   // DB7-Pin
}TLCD_NAME_t;

#define  TLCD_ANZ   6 // Anzahl von TLCD_NAME_t


//--------------------------------------------------------------
// Struktur eines Pins
//--------------------------------------------------------------
typedef struct {
  TLCD_NAME_t TLCD_NAME;   // Name
  GPIO_TypeDef* TLCD_PORT; // Port
  const uint16_t TLCD_PIN; // Pin
  uint32_t TLCD_INIT;     // Init
}LCD_2X16_t;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void lcdInit(void);
void lcdClear(void);
void lcdSetMode(TLCD_MODE_t mode);
void lcdPutsAt(uint8_t x, uint8_t y, char *ptr);
void lcdPutsCG(uint8_t nr, uint8_t *pixeldata);
void lcdPutsCGAt(uint8_t x, uint8_t y, uint8_t nr);
void lcdCreateCustomChar(uint8_t id, uint8_t data[8]);
void lcdMoveCursor(uint8_t col, uint8_t row);


#ifdef __cplusplus
}
#endif


//--------------------------------------------------------------
#endif // __STM32F4_UB_LCD_2X16_H