################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include system/Src/stm32f4-hal/subdir.mk
-include system/Src/newlib/subdir.mk
-include system/Src/diag/subdir.mk
-include system/Src/cortexm/subdir.mk
-include system/Src/cmsis/subdir.mk
-include Src/HAL/subdir.mk
-include Src/subdir.mk
-include MTB_Cerb40/Middlewares/Third_Party/FatFs/src/option/subdir.mk
-include MTB_Cerb40/Middlewares/Third_Party/FatFs/src/drivers/subdir.mk
-include MTB_Cerb40/Middlewares/Third_Party/FatFs/src/subdir.mk
-include MTB_Cerb40/Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
-include MTB_Cerb40/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
MotorTestBench\ V4.hex \

SECONDARY_SIZE += \
MotorTestBench\ V4.siz \


# All Target
all: MotorTestBench\ V4.elf secondary-outputs

# Tool invocations
MotorTestBench\ V4.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-move-loop-invariants -Wall -Wextra  -g3 -T mem.ld -T libs.ld -T sections.ld -nostartfiles -Xlinker --gc-sections -L"../ldscripts" -Wl,-Map,"MotorTestBench V4.map" --specs=nano.specs -u _printf_float -o "MotorTestBench V4.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

MotorTestBench\ V4.hex: MotorTestBench\ V4.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "MotorTestBench V4.elf"  "MotorTestBench V4.hex"
	@echo 'Finished building: $@'
	@echo ' '

MotorTestBench\ V4.siz: MotorTestBench\ V4.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "MotorTestBench V4.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS)$(CPP_DEPS) "MotorTestBench V4.elf"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
