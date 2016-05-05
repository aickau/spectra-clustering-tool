################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/AFAMain.c \
../src/AFANetworkSettings.c \
../src/AFAProcessing.c \
../src/AFAProcessingHWC-Simu.c \
../src/AFAProcessingHWWrapper.c \
../src/AFARandom.c \
../src/AFASpectra.c \
../src/AFATypes.c \
../src/BoardIO.c \
../src/Interrupt.c \
../src/main.c \
../src/platform.c \
../src/xgpio_tapp_example.c 

OBJS += \
./src/AFAMain.o \
./src/AFANetworkSettings.o \
./src/AFAProcessing.o \
./src/AFAProcessingHWC-Simu.o \
./src/AFAProcessingHWWrapper.o \
./src/AFARandom.o \
./src/AFASpectra.o \
./src/AFATypes.o \
./src/BoardIO.o \
./src/Interrupt.o \
./src/main.o \
./src/platform.o \
./src/xgpio_tapp_example.o 

C_DEPS += \
./src/AFAMain.d \
./src/AFANetworkSettings.d \
./src/AFAProcessing.d \
./src/AFAProcessingHWC-Simu.d \
./src/AFAProcessingHWWrapper.d \
./src/AFARandom.d \
./src/AFASpectra.d \
./src/AFATypes.d \
./src/BoardIO.d \
./src/Interrupt.d \
./src/main.d \
./src/platform.d \
./src/xgpio_tapp_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O2 -c -fmessage-length=0 -MT"$@" -I../../JSC_1_BSP/microblaze_0/include -mlittle-endian -mcpu=v9.5 -mxl-soft-mul -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


