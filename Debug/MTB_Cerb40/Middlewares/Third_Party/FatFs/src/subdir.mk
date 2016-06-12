################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MTB_Cerb40/Middlewares/Third_Party/FatFs/src/diskio.c \
../MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff.c \
../MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c 

OBJS += \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/diskio.o \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff.o \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff_gen_drv.o 

C_DEPS += \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/diskio.d \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff.d \
./MTB_Cerb40/Middlewares/Third_Party/FatFs/src/ff_gen_drv.d 


# Each subdirectory must supply rules for building sources it contributes
MTB_Cerb40/Middlewares/Third_Party/FatFs/src/%.o: ../MTB_Cerb40/Middlewares/Third_Party/FatFs/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DSTM32F405xx -DUSE_HAL_DRIVER -DHSE_VALUE=12000000 -DOS_USE_SEMIHOSTING -DOS_USE_TRACE_SEMIHOSTING_STDOUT -I"../Inc" -I"../system/Inc" -I"../system/Inc/cmsis" -I"../system/Inc/stm32f4-hal" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Class\CDC\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\ST\STM32_USB_Device_Library\Core\Inc" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src" -I"C:\code\Test\MotorTestBench V4\MTB_Cerb40\Middlewares\Third_Party\FatFs\src\drivers" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


