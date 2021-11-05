################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StoringSettings/storingSettings.c 

OBJS += \
./StoringSettings/storingSettings.o 

C_DEPS += \
./StoringSettings/storingSettings.d 


# Each subdirectory must supply rules for building sources it contributes
StoringSettings/%.o: ../StoringSettings/%.c StoringSettings/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Inc -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Fans" -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Fans" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Display" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/fillMenu" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Keyboard" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/ssd1306" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/StoringSettings" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

