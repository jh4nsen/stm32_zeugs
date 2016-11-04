################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f303xc.s 

OBJS += \
./startup/startup_stm32f303xc.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=soft -I"/home/me/workspace/stm32f3discovery_hal_lib" -I"/home/me/workspace/test/inc" -I"/home/me/workspace/stm32f3discovery_hal_lib/CMSIS/core" -I"/home/me/workspace/stm32f3discovery_hal_lib/CMSIS/device" -I"/home/me/workspace/stm32f3discovery_hal_lib/Utilities/Components/l3gd20" -I"/home/me/workspace/stm32f3discovery_hal_lib/Utilities/Components/lsm303dlhc" -I"/home/me/workspace/stm32f3discovery_hal_lib/Utilities/Components/Common" -I"/home/me/workspace/stm32f3discovery_hal_lib/Utilities/STM32F3-Discovery" -I"/home/me/workspace/stm32f3discovery_hal_lib/HAL_Driver/Inc" -I"/home/me/workspace/stm32f3discovery_hal_lib/HAL_Driver/Inc/Legacy" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


