/*
 * data.h
 *
 *  Created on: 08.05.2016
 *      Author: Henry
 */

#ifndef DATA_H_
#define DATA_H_


#include <stdint.h>

typedef struct {
	//	Dynamic sensor and actuator data
	float		voltage;		//	in volts
	float		current;		//	in ampere
	float		power;			//	in watt
	int16_t		rpm;			//	in rotations per second
	float		thrust;			//	in gramms
	int16_t		gas;			//	gas in percent (control of ESC)
	int32_t		thrust_raw;		//	raw value of thurst sensor
	bool		measure_offsets;//	will be true as long as gas is 0, in order to continuously measure any offsets

	//	Devices attached
	bool		sd_card_attached;	//	If SD card is attached
	bool		battery_attached;	//	If voltage is > 9V
	bool		usb_attached;		//	If a usb connection has been detected

} systemData_t;

typedef struct {
	int32_t	thrust_offset;
	float	thrust_calibration_factor;
	int32_t thrust_calibration_divisorN;
	int32_t thrust_calibration_factorN;
	uint16_t	display_contrast;
} systemSettings_t;


extern systemData_t	systemData;
extern systemSettings_t systemSettings;

extern void systemSettingsInit();
extern void systemSettingsLoad();
extern void systemSettingsSave();


#endif /* DATA_H_ */
