################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/cr_startup_lpc17.c \
../src/debug.c \
../src/main.c 

OBJS += \
./src/adc.o \
./src/cr_startup_lpc17.o \
./src/debug.o \
./src/main.o 

C_DEPS += \
./src/adc.d \
./src/cr_startup_lpc17.d \
./src/debug.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC17xx -DDEBUG -D__CODE_RED -D__REDLIB__ -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\CMSISv1p30_LPC17xx\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\lib_small_printf_m3\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\adc_semihosting\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


