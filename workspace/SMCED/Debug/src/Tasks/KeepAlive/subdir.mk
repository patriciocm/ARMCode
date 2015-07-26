################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Tasks/KeepAlive/taskKeepAlive.c 

OBJS += \
./src/Tasks/KeepAlive/taskKeepAlive.o 

C_DEPS += \
./src/Tasks/KeepAlive/taskKeepAlive.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tasks/KeepAlive/%.o: ../src/Tasks/KeepAlive/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"/home/fanaur/Desktop/workspace/CMSISv2p00_LPC17xx/inc" -I"/home/fanaur/Desktop/workspace/SMCED/src/Tasks/WebServer" -I"/home/fanaur/Desktop/workspace/SMCED/src/Tasks/SdCard/lib_FatFS/inc" -I"/home/fanaur/Desktop/workspace/SMCED/src/Tasks/SdCard" -I"/home/fanaur/Desktop/workspace/SMCED/src/Tasks/Adc" -I"/home/fanaur/Desktop/workspace/libDriversMcuPeripherals/inc/Lib_MCU" -I"/home/fanaur/Desktop/workspace/libDriversMcuPeripherals/inc/Lib_EaBaseBoard" -I"/home/fanaur/Desktop/workspace/SMCED/src" -I"/home/fanaur/Desktop/workspace/SMCED/src/Tasks" -I"/home/fanaur/Desktop/workspace/SMCED/FreeRTOS_include" -I"/home/fanaur/Desktop/workspace/SMCED/FreeRTOS_portable" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


