################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/TASK_MODBUS_MASTER.c \
../Source/TASK_WEB_SERVICE.c \
../Source/cr_startup_lpc176x.c \
../Source/main.c \
../Source/printf-stdarg.c 

OBJS += \
./Source/TASK_MODBUS_MASTER.o \
./Source/TASK_WEB_SERVICE.o \
./Source/cr_startup_lpc176x.o \
./Source/main.o \
./Source/printf-stdarg.o 

C_DEPS += \
./Source/TASK_MODBUS_MASTER.d \
./Source/TASK_WEB_SERVICE.d \
./Source/cr_startup_lpc176x.d \
./Source/main.d \
./Source/printf-stdarg.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -lws2_32 -D__NEWLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -DWITH_LEAN -DWITH_LEANER -DWITH_NOIO -DIPv4 -DLWIP_DEBUG -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\LibModbus\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\Gsoap\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include\ipv4" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\netif\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\lwIP_Apps" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include" -I"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Source/cr_startup_lpc176x.o: ../Source/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -lws2_32 -D__NEWLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -DWITH_LEAN -DWITH_LEANER -DWITH_NOIO -DIPv4 -DLWIP_DEBUG -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\LibModbus\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\Gsoap\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include\ipv4" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\netif\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\lwIP_Apps" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include" -I"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source" -Os -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"Source/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


