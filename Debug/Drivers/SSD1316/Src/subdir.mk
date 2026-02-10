################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SSD1316/Src/ssd1316_adv_graphics.c \
../Drivers/SSD1316/Src/ssd1316_core.c \
../Drivers/SSD1316/Src/ssd1316_drawing.c \
../Drivers/SSD1316/Src/ssd1316_hw_config.c 

OBJS += \
./Drivers/SSD1316/Src/ssd1316_adv_graphics.o \
./Drivers/SSD1316/Src/ssd1316_core.o \
./Drivers/SSD1316/Src/ssd1316_drawing.o \
./Drivers/SSD1316/Src/ssd1316_hw_config.o 

C_DEPS += \
./Drivers/SSD1316/Src/ssd1316_adv_graphics.d \
./Drivers/SSD1316/Src/ssd1316_core.d \
./Drivers/SSD1316/Src/ssd1316_drawing.d \
./Drivers/SSD1316/Src/ssd1316_hw_config.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1316/Src/%.o Drivers/SSD1316/Src/%.su Drivers/SSD1316/Src/%.cyclo: ../Drivers/SSD1316/Src/%.c Drivers/SSD1316/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SSD1316-2f-Src

clean-Drivers-2f-SSD1316-2f-Src:
	-$(RM) ./Drivers/SSD1316/Src/ssd1316_adv_graphics.cyclo ./Drivers/SSD1316/Src/ssd1316_adv_graphics.d ./Drivers/SSD1316/Src/ssd1316_adv_graphics.o ./Drivers/SSD1316/Src/ssd1316_adv_graphics.su ./Drivers/SSD1316/Src/ssd1316_core.cyclo ./Drivers/SSD1316/Src/ssd1316_core.d ./Drivers/SSD1316/Src/ssd1316_core.o ./Drivers/SSD1316/Src/ssd1316_core.su ./Drivers/SSD1316/Src/ssd1316_drawing.cyclo ./Drivers/SSD1316/Src/ssd1316_drawing.d ./Drivers/SSD1316/Src/ssd1316_drawing.o ./Drivers/SSD1316/Src/ssd1316_drawing.su ./Drivers/SSD1316/Src/ssd1316_hw_config.cyclo ./Drivers/SSD1316/Src/ssd1316_hw_config.d ./Drivers/SSD1316/Src/ssd1316_hw_config.o ./Drivers/SSD1316/Src/ssd1316_hw_config.su

.PHONY: clean-Drivers-2f-SSD1316-2f-Src

