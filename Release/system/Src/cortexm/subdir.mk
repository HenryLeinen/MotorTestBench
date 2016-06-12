################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/Src/cortexm/_initialize_hardware.c \
../system/Src/cortexm/_reset_hardware.c \
../system/Src/cortexm/exception_handlers.c 

OBJS += \
./system/Src/cortexm/_initialize_hardware.o \
./system/Src/cortexm/_reset_hardware.o \
./system/Src/cortexm/exception_handlers.o 

C_DEPS += \
./system/Src/cortexm/_initialize_hardware.d \
./system/Src/cortexm/_reset_hardware.d \
./system/Src/cortexm/exception_handlers.d 


# Each subdirectory must supply rules for building sources it contributes
system/Src/cortexm/%.o: ../system/Src/cortexm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall -Wextra  -g -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Drivers\CMSIS\Include" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


