################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Tasks/WebServer/ethmac.c \
../src/Tasks/WebServer/taskWebServer.c \
../src/Tasks/WebServer/tcpip.c 

OBJS += \
./src/Tasks/WebServer/ethmac.o \
./src/Tasks/WebServer/taskWebServer.o \
./src/Tasks/WebServer/tcpip.o 

C_DEPS += \
./src/Tasks/WebServer/ethmac.d \
./src/Tasks/WebServer/taskWebServer.d \
./src/Tasks/WebServer/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tasks/WebServer/%.o: ../src/Tasks/WebServer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -DPACK_STRUCT_END=__attribute\(\(packed\)\) -DGCC_ARMCM3 -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/CMSISv2p00_LPC17xx/inc" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks/WebServer" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks/SdCard/lib_FatFS/inc" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks/SdCard" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks/SerialMemory" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks/Adc" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/libDriversMcuPeripherals/inc/Lib_MCU" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/libDriversMcuPeripherals/inc/Lib_EaBaseBoard" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/src/Tasks" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/FreeRTOS_include" -I"/home/fanaur/RepositoriosMercurial/trabajofinalsisemb/Codigo/workspaceLpc/SMCED/FreeRTOS_portable" -O0 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


