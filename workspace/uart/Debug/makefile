################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: uart.axf

# Tool invocations
uart.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\nxp\LPCXpresso_5.2.4_2122\workspace\CMSISv1p30_LPC17xx\Debug" -L"C:\nxp\LPCXpresso_5.2.4_2122\workspace\lib_small_printf_m3\Lib" -Xlinker -Map=uart.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "uart_Debug.ld" -o "uart.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) uart.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-cat ../Docs/Abstract.txt; arm-none-eabi-size uart.axf; # arm-none-eabi-objcopy -O binary uart.axf uart.bin ; checksum -d uart.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
