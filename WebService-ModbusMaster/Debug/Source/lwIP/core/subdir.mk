################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/lwIP/core/def.c \
../Source/lwIP/core/dhcp.c \
../Source/lwIP/core/dns.c \
../Source/lwIP/core/init.c \
../Source/lwIP/core/lwip_timers.c \
../Source/lwIP/core/mem.c \
../Source/lwIP/core/memp.c \
../Source/lwIP/core/netif.c \
../Source/lwIP/core/pbuf.c \
../Source/lwIP/core/raw.c \
../Source/lwIP/core/stats.c \
../Source/lwIP/core/sys.c \
../Source/lwIP/core/tcp.c \
../Source/lwIP/core/tcp_in.c \
../Source/lwIP/core/tcp_out.c \
../Source/lwIP/core/udp.c 

OBJS += \
./Source/lwIP/core/def.o \
./Source/lwIP/core/dhcp.o \
./Source/lwIP/core/dns.o \
./Source/lwIP/core/init.o \
./Source/lwIP/core/lwip_timers.o \
./Source/lwIP/core/mem.o \
./Source/lwIP/core/memp.o \
./Source/lwIP/core/netif.o \
./Source/lwIP/core/pbuf.o \
./Source/lwIP/core/raw.o \
./Source/lwIP/core/stats.o \
./Source/lwIP/core/sys.o \
./Source/lwIP/core/tcp.o \
./Source/lwIP/core/tcp_in.o \
./Source/lwIP/core/tcp_out.o \
./Source/lwIP/core/udp.o 

C_DEPS += \
./Source/lwIP/core/def.d \
./Source/lwIP/core/dhcp.d \
./Source/lwIP/core/dns.d \
./Source/lwIP/core/init.d \
./Source/lwIP/core/lwip_timers.d \
./Source/lwIP/core/mem.d \
./Source/lwIP/core/memp.d \
./Source/lwIP/core/netif.d \
./Source/lwIP/core/pbuf.d \
./Source/lwIP/core/raw.d \
./Source/lwIP/core/stats.d \
./Source/lwIP/core/sys.d \
./Source/lwIP/core/tcp.d \
./Source/lwIP/core/tcp_in.d \
./Source/lwIP/core/tcp_out.d \
./Source/lwIP/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
Source/lwIP/core/%.o: ../Source/lwIP/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -lws2_32 -D__NEWLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -DWITH_LEAN -DWITH_LEANER -DWITH_NOIO -DIPv4 -DLWIP_DEBUG -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\LibModbus\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\Gsoap\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include\ipv4" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\netif\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\lwIP_Apps" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include" -I"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


