//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "diag/Trace.h"

#include "gpio.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "sdio.h"
#include "fatfs.h"
#include "usb_device.h"
#include "bsp_driver_sd.h"
#include "tim.h"

#include "display.h"
#include "data.h"
#include "rot_enc.h"
#include "esc.h"
#include "power_sense.h"
#include "HX711.h"
#include "mxconstants.h"
#include "rpm.h"


// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//


FATFS	FS;
bool	mounted = false;
HX711	thrust_sensor;


void ledSetOutputRaw(uint16_t raw) {
	TIM_OC_InitTypeDef sConfigOC;

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = raw;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
}


//	This function implements the optical feedback using the hearbeat LED
//	Shall be called cyclically
void ledUpdatePwm(void) {
	static uint16_t cnt = 0;
	static uint8_t dir = 1;

	//	Ramp up ?
	if (dir == 1) {
		cnt +=5;
		if (cnt ==150)
			dir = 0;
	} else {	//	Ramp down
		cnt-=5;
		if (cnt == 0)
			dir = 1;
	}
	ledSetOutputRaw(cnt);
}

//	This function reads and processes all sensor inputs
//	and updates the system variables structure
void processSensors(void) {
	static float thrust_actual;

	//	Read the thrust sensor
	thrust_sensor.readSensor(thrust_actual, ChannelA_Gain128, false);
	systemData.thrust_raw = (int32_t)thrust_actual;

	//	Get new RPM value
//	systemData.rpm = (int16_t)rpmGetRPSValue();
	systemData.rpm = (int16_t)rpmGetRPMValue();

	//	Get new voltage and current values
	systemData.voltage = powerSenseGetVoltage();
	systemData.current = powerSenseGetCurrent();
	if (systemData.current > 9.9f)
		systemData.current = 9.9f;
	systemData.power = systemData.voltage * systemData.current;

	//	Process thrust and power sensor values in case the system shall be idle
	if (systemData.measure_offsets) {
		//	Reset all thrust offset and value
		systemSettings.thrust_offset = thrust_actual;
		powerSenseGetNewOffset();
	}
	systemData.thrust = fabs(thrust_actual - systemSettings.thrust_offset) / systemSettings.thrust_calibration_factor;

	//	Capture error events
	if (systemData.thrust >= 10000)
		systemData.thrust = 9999;
	if (systemData.thrust < 0)
		systemData.thrust = 0;

	//	Check if devices are attached
	if (systemData.voltage > 5.0f)
		systemData.battery_attached = true;
	else
		systemData.battery_attached = false;

	//	Checking for SD card needs some timer to wait after the card was detected
	static uint32_t TickInserted = 0;	//	This will be set when we detect the card first
	if (BSP_SD_IsDetected() == SD_PRESENT) {
		//	SD card is inside, check to see if we already mounted it
		if (!mounted) {
			//	SD card has not yet been mounted, so mount it after time x
			if (TickInserted == 0) {
				//	we have to wait for some time to allow the card to get stable voltage, before we actually mount it
				TickInserted = HAL_GetTick();
				trace_puts("SD card detected !");
			} else {
				//	Is the time (500ms) already elapsed ?
				if ((HAL_GetTick() - TickInserted) > 150000){
					BSP_SD_Init();
					if (f_mount(&FS, "0:", 1) == FR_OK) {
						mounted = true;
						HAL_SD_CardInfoTypedef	ci;
						BSP_SD_GetCardInfo(&ci);
						trace_printf("SD Card mounted. Card has capacity of %ld Bytes", ci.CardCapacity);
						systemData.sd_card_attached = true;
					} else {
						mounted = false;
						TickInserted = 0;
						trace_printf("Failed to mount SD card !\n");
						systemData.sd_card_attached = false;
					}
				}
			}
		}
	} else {
		if (mounted) {
//			mounted = false;
//			f_mount(&FS, "0:", 0);
//			trace_puts("Unmounting SD Card!\n");
//			TickInserted = 0;
//			systemData.sd_card_attached = false;
		}
	}
}


//	HAL initialization
void halInit(void) {
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SDIO_SD_Init();
	MX_ADC1_Init();
	MX_DAC_Init();
	MX_USB_DEVICE_Init();
	MX_FATFS_Init();
	MX_TIM4_Init();

	SysTick_Config(SystemCoreClock / 1000000);
}

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

extern MenuEntry_t	MainMenuStruct[];

int
main(int argc, char* argv[])
{
	displayMenu		display(MainMenuStruct);

	trace_puts("Running!");

	// At this stage the system clock should have already been configured
	// at high speed.
	halInit();

	//	Initialize the data objects
	systemSettingsInit();

	//	Initialize the SD Card
	BSP_SD_Init();

	//	Start the rotary encoder (TIM)
	rotEncInit();

	//	activate blue led (GPIO)
	ledSetOutputRaw(5);

	//	Initialize the display (GPIO)
	display.init();

	//	Initialize the thrust sensor (GPIO)
	thrust_sensor.init();

	//	Initialize the ESC (TIMER)
	escInit();

	//	Initialize the power sense (ADC)
	powerSenseInit();

	//	Initialize the RPM measurement (TIM)
	rpmInit();

	while(1) {
		HAL_Delay(50000);

		//	Update the heartbeat LED
		ledUpdatePwm();

		//	Read and process all sensors --> will be updated in the system data set
		processSensors();

		//	Query the button state
		display.processMenu(rotEncButtonPressed(), rotEncGetScroll());

		//	Set new ESC output
		escSetOutput(systemData.gas);

		//	Update Display Contrast
		display.displaySetContrast(systemSettings.display_contrast);

		//	Check if SD card is detected
#if 0
		if (!mounted && (BSP_SD_IsDetected() == SD_PRESENT)) {
			//	Card detected, so mount the file system if not already done
			if (f_mount(&FS, "0:", 1) == FR_OK) {
				mounted = true;
				HAL_SD_CardInfoTypedef	ci;
				BSP_SD_GetCardInfo(&ci);
				FIL fil;
				if (f_open(&fil, "MyTest.txt", FA_CREATE_NEW|FA_WRITE) == FR_OK)
				{
					f_write(&fil, (const void*)"Dies ist ein test !\r\n", 21, NULL);
					f_close(&fil);
					f_mount(&FS, "0:", 0);
				}
			} else {
				mouted = false;
			}
		}
#endif
	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
