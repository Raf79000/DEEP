################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/clock.c \
../src/data.c \
../src/main.c \
../src/telemetre.c 

OBJS += \
./src/clock.o \
./src/data.o \
./src/main.o \
./src/telemetre.o 

C_DEPS += \
./src/clock.d \
./src/data.d \
./src/main.d \
./src/telemetre.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -I"C:/Users/0302331W/Documents/DEEP/software/include" -I"C:/Users/0302331W/Documents/DEEP/software/src" -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src

clean-src:
	-$(RM) ./src/clock.cyclo ./src/clock.d ./src/clock.o ./src/clock.su ./src/data.cyclo ./src/data.d ./src/data.o ./src/data.su ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su ./src/telemetre.cyclo ./src/telemetre.d ./src/telemetre.o ./src/telemetre.su

.PHONY: clean-src

