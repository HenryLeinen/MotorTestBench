/*
 * power_sense.h
 *
 *  Created on: 09.05.2016
 *      Author: Henry
 */

#ifndef POWER_SENSE_H_
#define POWER_SENSE_H_


extern void powerSenseInit(void);
extern float powerSenseGetCurrent(void);
extern float powerSenseGetVoltage(void);
extern void powerSenseGetNewOffset(void);



#endif /* POWER_SENSE_H_ */
