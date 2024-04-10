################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_DriverInterface.c \
../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_IOUtils.c \
../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CharQueueTxAndRx.c \
../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CircularBufferRx.c \
../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_ZeroCopyTx.c 

OBJS += \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_DriverInterface.o \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_IOUtils.o \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CharQueueTxAndRx.o \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CircularBufferRx.o \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_ZeroCopyTx.o 

C_DEPS += \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_DriverInterface.d \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/FreeRTOS_IOUtils.d \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CharQueueTxAndRx.d \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_CircularBufferRx.d \
./Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/IOUtils_ZeroCopyTx.d 


# Each subdirectory must supply rules for building sources it contributes
Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/%.o: ../Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -lws2_32 -D__NEWLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -DWITH_LEAN -DWITH_LEANER -DWITH_NOIO -DIPv4 -DLWIP_DEBUG -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\LibModbus\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\Gsoap\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-IO\Device\LPC17xx\SupportedBoards" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS\portable\GCC\ARM_CM3" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\FreeRTOS-Products\FreeRTOS-Plus-CLI" -I"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\inc" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include\ipv4" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\netif\include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\lwIP_Apps" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source\lwIP\include" -I"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Include" -I"D:\LPC-Proyectos\BORRAR\WebService-ModbusMaster\Source" -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -Wextra -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


