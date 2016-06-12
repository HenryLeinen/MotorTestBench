################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/_initialize_hardware.c \
../Src/_write.c \
../Src/lcd.c \
../Src/stm32f4xx_hal_msp.c 

CPP_SRCS += \
../Src/HX711.cpp \
../Src/data.cpp \
../Src/display.cpp \
../Src/esc.cpp \
../Src/main.cpp \
../Src/menuHandlers.cpp \
../Src/menuLoadCalibration.cpp \
../Src/menuSaveCalibration.cpp \
../Src/power_sense.cpp \
../Src/rot_enc.cpp \
../Src/rpm.cpp 

OBJS += \
./Src/HX711.o \
./Src/_initialize_hardware.o \
./Src/_write.o \
./Src/data.o \
./Src/display.o \
./Src/esc.o \
./Src/lcd.o \
./Src/main.o \
./Src/menuHandlers.o \
./Src/menuLoadCalibration.o \
./Src/menuSaveCalibration.o \
./Src/power_sense.o \
./Src/rot_enc.o \
./Src/rpm.o \
./Src/stm32f4xx_hal_msp.o 

C_DEPS += \
./Src/_initialize_hardware.d \
./Src/_write.d \
./Src/lcd.d \
./Src/stm32f4xx_hal_msp.d 

CPP_DEPS += \
./Src/HX711.d \
./Src/data.d \
./Src/display.d \
./Src/esc.d \
./Src/main.d \
./Src/menuHandlers.d \
./Src/menuLoadCalibration.d \
./Src/menuSaveCalibration.d \
./Src/power_sense.d \
./Src/rot_enc.d \
./Src/rpm.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C++ Compiler'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu++11 -fabi-version=0 -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/stm32f4xx_hal_msp.o: ../Src/stm32f4xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu11 -Wno-missing-prototypes -Wno-missing-declarations -MMD -MP -MF"$(@:%.o=%.d)" -MT"Src/stm32f4xx_hal_msp.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


