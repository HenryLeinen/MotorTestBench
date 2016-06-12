/*
 * rot_enc.h
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */

#ifndef INC_ROT_ENC_H_
#define INC_ROT_ENC_H_

/* ------------ BEGIN CONFIGURATION SECTION --------------*/
 //#define 	ROT_ENC_TIMER_DEVICE			TIM1
 //#define 	ROT_ENC_TIMER_DEVICE			TIM2
#define		ROT_ENC_TIMER_DEVICE			TIM3
 //#define 	ROT_ENC_TIMER_DEVICE			TIM4
 //#define 	ROT_ENC_TIMER_DEVICE			TIM5
 //#define 	ROT_ENC_TIMER_DEVICE			TIM8

#define		ROT_ENC_PIN_A_PORT				GPIOC
#define		ROT_ENC_PIN_A_PIN				GPIO_PIN_6

#define		ROT_ENC_PIN_B_PORT				GPIOA
#define		ROT_ENC_PIN_B_PIN				GPIO_PIN_7

/* ----------- END OF CONFIGURATIOn SECTION --------------*/



extern void rotEncInit(void);
extern uint16_t rotEncGetPosition(void);
extern int16_t rotEncGetScroll(void);
extern bool rotEncButtonPressed(void);



#endif /* INC_ROT_ENC_H_ */
