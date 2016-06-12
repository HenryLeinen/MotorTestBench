/*
 * HX711.h
 *
 *  Created on: 06.05.2016
 *      Author: Henry
 */

#ifndef HX711_H_
#define HX711_H_

#include <stdint.h>

typedef enum {
	ChannelA_Gain128 = 25,
	ChannelB_Gain32 = 26,
	ChannelA_Gain64 = 27
} HX711_Channel_t;

class HX711 {
public:
	HX711() {}
	virtual ~HX711();

	void init();

	bool readSensor(float& val, HX711_Channel_t channel = ChannelA_Gain128, bool bWait = true);
};

#endif /* HX711_H_ */
