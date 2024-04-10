################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/lwIP/api/api_lib.c \
../Source/lwIP/api/api_msg.c \
../Source/lwIP/api/err.c \
../Source/lwIP/api/netbuf.c \
../Source/lwIP/api/netdb.c \
../Source/lwIP/api/netifapi.c \
../Source/lwIP/api/sockets.c \
../Source/lwIP/api/tcpip.c 

OBJS += \
./Source/lwIP/api/api_lib.o \
./Source/lwIP/api/api_msg.o \
./Source/lwIP/api/err.o \
./Source/lwIP/api/netbuf.o \
./Source/lwIP/api/netdb.o \
./Source/lwIP/api/netifapi.o \
./Source/lwIP/api/sockets.o \
./Source/lwIP/api/tcpip.o 

C_DEPS += \
./Source/lwIP/api/api_lib.d \
./Source/lwIP/api/api_msg.d \
./Source/lwIP/api/err.d \
./Source/lwIP/api/netbuf.d \
./Source/lwIP/api/netdb.d \
./Source/lwIP/api/netifapi.d \
./Source/lwIP/api/sockets.d \
./Source/lwIP/api/tcpip.d 


# Each subdirectory must supply rules for building sources it contributes
Source/lwIP/api/%.o: ../Source/lwIP/api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -lws2_32 -D__NEWLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -DWITH_LEAN -DWITH_LEANER -DWITH_NOIO -DIPv4 -DLWIP_DEBUG -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\LibModbus\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\Gsoap\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include\ipv4" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\netif\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\lwIP_Apps" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include" -I"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


