################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/lwIP/netif/subdir.mk
-include Source/lwIP/lwIP_Apps/apps/httpserver_raw/subdir.mk
-include Source/lwIP/lwIP_Apps/apps/BasicSocketCommandServer/subdir.mk
-include Source/lwIP/lwIP_Apps/subdir.mk
-include Source/lwIP/core/ipv4/subdir.mk
-include Source/lwIP/core/subdir.mk
-include Source/lwIP/api/subdir.mk
-include Source/MemMang/subdir.mk
-include Source/LibModbus/src/subdir.mk
-include Source/Gsoap/src/subdir.mk
-include Source/FreeRTOS-Products/FreeRTOS/portable/GCC/ARM_CM3/subdir.mk
-include Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Device/LPC17xx/subdir.mk
-include Source/FreeRTOS-Products/FreeRTOS-Plus-IO/Common/subdir.mk
-include Source/FreeRTOS-Products/FreeRTOS/subdir.mk
-include Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: WebService-ModbusMaster.axf

# Tool invocations
WebService-ModbusMaster.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"D:\LPC-Proyectos\BORRAR\Lib_MCU_LPC17xx\Debug" -L"D:\LPC-Proyectos\BORRAR\CMSISv2p00_LPC17xx\Debug" -Xlinker -Map=WebService-ModbusMaster.map -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "WebService-ModbusMaster_Debug.ld" -o "WebService-ModbusMaster.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) WebService-ModbusMaster.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size WebService-ModbusMaster.axf; # arm-none-eabi-objcopy -O binary WebService-ModbusMaster.axf WebService-ModbusMaster.bin ; checksum -p LPC1769 -d WebService-ModbusMaster.bin;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
