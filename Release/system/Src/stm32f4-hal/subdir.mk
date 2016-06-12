################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/Src/stm32f4-hal/stm32f4xx_hal.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_adc.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_adc_ex.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_cortex.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_dac.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_dac_ex.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_dma.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_dma_ex.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_flash.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_gpio.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_iwdg.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_pcd.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_pcd_ex.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_pwr.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_rcc.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_sd.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_tim.c \
../system/Src/stm32f4-hal/stm32f4xx_hal_tim_ex.c \
../system/Src/stm32f4-hal/stm32f4xx_ll_sdmmc.c \
../system/Src/stm32f4-hal/stm32f4xx_ll_usb.c 

OBJS += \
./system/Src/stm32f4-hal/stm32f4xx_hal.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_adc.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_adc_ex.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_cortex.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_dac.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_dac_ex.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_dma.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_dma_ex.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_flash.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_gpio.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_iwdg.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_pcd.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_pcd_ex.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_pwr.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_rcc.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_sd.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_tim.o \
./system/Src/stm32f4-hal/stm32f4xx_hal_tim_ex.o \
./system/Src/stm32f4-hal/stm32f4xx_ll_sdmmc.o \
./system/Src/stm32f4-hal/stm32f4xx_ll_usb.o 

C_DEPS += \
./system/Src/stm32f4-hal/stm32f4xx_hal.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_adc.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_adc_ex.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_cortex.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_dac.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_dac_ex.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_dma.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_dma_ex.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_flash.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_gpio.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_iwdg.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_pcd.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_pcd_ex.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_pwr.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_rcc.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_sd.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_tim.d \
./system/Src/stm32f4-hal/stm32f4xx_hal_tim_ex.d \
./system/Src/stm32f4-hal/stm32f4xx_ll_sdmmc.d \
./system/Src/stm32f4-hal/stm32f4xx_ll_usb.d 


# Each subdirectory must supply rules for building sources it contributes
system/Src/stm32f4-hal/%.o: ../system/Src/stm32f4-hal/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu11 -Wno-bad-function-cast -Wno-conversion -Wno-sign-conversion -Wno-unused-parameter -Wno-sign-compare -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


