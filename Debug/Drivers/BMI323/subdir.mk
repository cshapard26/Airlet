################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BMI323/bmi323.c 

OBJS += \
./Drivers/BMI323/bmi323.o 

C_DEPS += \
./Drivers/BMI323/bmi323.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BMI323/%.o Drivers/BMI323/%.su Drivers/BMI323/%.cyclo: ../Drivers/BMI323/%.c Drivers/BMI323/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BMI323

clean-Drivers-2f-BMI323:
	-$(RM) ./Drivers/BMI323/bmi323.cyclo ./Drivers/BMI323/bmi323.d ./Drivers/BMI323/bmi323.o ./Drivers/BMI323/bmi323.su

.PHONY: clean-Drivers-2f-BMI323

