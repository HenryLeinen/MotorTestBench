/*
 * esc.h
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */

#ifndef ESC_H_
#define ESC_H_

#include <stdint.h>


 /* ------------ BEGIN CONFIGURATION SECTION --------------*/
#define		ESC_CONTROL_TIMER_CHANNEL			TIM_CHANNEL_1
//#define	ESC_CONTROL_TIMER_CHANNEL			TIM_CHANNEL_2
//#define	ESC_CONTROL_TIMER_CHANNEL			TIM_CHANNEL_3
//#define	ESC_CONTROL_TIMER_CHANNEL			TIM_CHANNEL_4

 /* ----------- END OF CONFIGURATIOn SECTION --------------*/


extern void escInit(void);
extern void escSetOutputRaw(uint16_t raw);
extern void escSetOutput(uint8_t percent);


#endif /* ESC_H_ */
