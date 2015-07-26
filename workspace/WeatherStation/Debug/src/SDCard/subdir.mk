################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SDCard/ccsbcs.c \
../src/SDCard/diskio.c \
../src/SDCard/fattime.c \
../src/SDCard/ff.c \
../src/SDCard/lpc17xx_clkpwr.c \
../src/SDCard/lpc17xx_gpio.c \
../src/SDCard/lpc17xx_libcfg_default.c \
../src/SDCard/lpc17xx_pinsel.c \
../src/SDCard/lpc17xx_ssp.c \
../src/SDCard/rtc_cal.c \
../src/SDCard/spi_sd_lpc17xx.c 

OBJS += \
./src/SDCard/ccsbcs.o \
./src/SDCard/diskio.o \
./src/SDCard/fattime.o \
./src/SDCard/ff.o \
./src/SDCard/lpc17xx_clkpwr.o \
./src/SDCard/lpc17xx_gpio.o \
./src/SDCard/lpc17xx_libcfg_default.o \
./src/SDCard/lpc17xx_pinsel.o \
./src/SDCard/lpc17xx_ssp.o \
./src/SDCard/rtc_cal.o \
./src/SDCard/spi_sd_lpc17xx.o 

C_DEPS += \
./src/SDCard/ccsbcs.d \
./src/SDCard/diskio.d \
./src/SDCard/fattime.d \
./src/SDCard/ff.d \
./src/SDCard/lpc17xx_clkpwr.d \
./src/SDCard/lpc17xx_gpio.d \
./src/SDCard/lpc17xx_libcfg_default.d \
./src/SDCard/lpc17xx_pinsel.d \
./src/SDCard/lpc17xx_ssp.d \
./src/SDCard/rtc_cal.d \
./src/SDCard/spi_sd_lpc17xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/SDCard/%.o: ../src/SDCard/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\WeatherStation\inc\SDCard" -I"C:\nxp\LPCXpresso_5.2.4_2122\workspace\lib_small_printf_m3\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


