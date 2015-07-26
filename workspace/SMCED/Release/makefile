################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/subdir.mk
-include src/Tasks/subdir.mk
-include src/Tasks/WebServer/subdir.mk
-include src/Tasks/SerialMemory/subdir.mk
-include src/Tasks/SdCard/subdir.mk
-include src/Tasks/SdCard/option/subdir.mk
-include src/Tasks/SdCard/lib_FatFS/src/option/subdir.mk
-include src/Tasks/SdCard/lib_FatFS/src/lpc1xxx_spi_sd/subdir.mk
-include src/Tasks/SdCard/lib_FatFS/src/subdir.mk
-include src/Tasks/PcCommunication/subdir.mk
-include src/Tasks/KeepAlive/subdir.mk
-include src/Tasks/Core/subdir.mk
-include src/Tasks/Adc/subdir.mk
-include FreeRTOS_src/subdir.mk
-include FreeRTOS_portable/subdir.mk
-include FreeRTOS_portable/MemMang/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SMCED.axf

# Tool invocations
SMCED.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/CMSISv2p00_LPC17xx/Debug" -L"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/libDriversMcuPeripherals/Debug" -Xlinker -Map="SMCED.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "SMCED_Release.ld" -o"SMCED.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) SMCED.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SMCED.axf"; # arm-none-eabi-objcopy -O binary "SMCED.axf" "SMCED.bin" ; checksum -p LPC1769 -d "SMCED.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
