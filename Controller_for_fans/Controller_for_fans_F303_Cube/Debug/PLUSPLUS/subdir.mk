################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PLUSPLUS/testPlus.cpp 

OBJS += \
./PLUSPLUS/testPlus.o 

CPP_DEPS += \
./PLUSPLUS/testPlus.d 


# Each subdirectory must supply rules for building sources it contributes
PLUSPLUS/%.o: ../PLUSPLUS/%.cpp PLUSPLUS/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F303xC -c -I../Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F3xx_HAL_Driver/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F3xx/Include -I../Drivers/CMSIS/Include -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Fans" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/PLUSPLUS" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Display" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/ssd1306" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Inc" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/fillMenu" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/StoringSettings" -I"Z:/Share/Real_project/Fans_controller_F303_Cube/Menu" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

