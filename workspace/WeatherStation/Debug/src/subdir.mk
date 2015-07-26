################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adc.c \
../src/auxiliares.c \
../src/cr_startup_lpc176x.c \
../src/main.c \
../src/rtc.c \
../src/timer.c \
../src/uart.c \
../src/xbee.c 

OBJS += \
./src/adc.o \
./src/auxiliares.o \
./src/cr_startup_lpc176x.o \
./src/main.o \
./src/rtc.o \
./src/timer.o \
./src/uart.o \
./src/xbee.o 

C_DEPS += \
./src/adc.d \
./src/auxiliares.d \
./src/cr_startup_lpc176x.d \
./src/main.d \
./src/rtc.d \
./src/timer.d \
./src/uart.d \
./src/xbee.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc\SDCard" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\lib_small_printf_m3\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/cr_startup_lpc176x.o: ../src/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc\SDCard" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\lib_small_printf_m3\inc" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


