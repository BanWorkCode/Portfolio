################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ssd1306/fonts.c \
../ssd1306/ssd1306.c 

OBJS += \
./ssd1306/fonts.o \
./ssd1306/ssd1306.o 

C_DEPS += \
./ssd1306/fonts.d \
./ssd1306/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
ssd1306/%.o: ../ssd1306/%.c ssd1306/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Inc -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/ssd1306" -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/Display" -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/Fans" -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/fillMenu" -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/Keyboard" -I"C:/Users/Maxim/Desktop/Fans_controller_F303_Cube/StoringSettings" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

