################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/tap-bridge/examples/tap-csma-virtual-machine.cc \
../src/tap-bridge/examples/tap-csma.cc \
../src/tap-bridge/examples/tap-wifi-dumbbell.cc \
../src/tap-bridge/examples/tap-wifi-virtual-machine.cc 

OBJS += \
./src/tap-bridge/examples/tap-csma-virtual-machine.o \
./src/tap-bridge/examples/tap-csma.o \
./src/tap-bridge/examples/tap-wifi-dumbbell.o \
./src/tap-bridge/examples/tap-wifi-virtual-machine.o 

CC_DEPS += \
./src/tap-bridge/examples/tap-csma-virtual-machine.d \
./src/tap-bridge/examples/tap-csma.d \
./src/tap-bridge/examples/tap-wifi-dumbbell.d \
./src/tap-bridge/examples/tap-wifi-virtual-machine.d 


# Each subdirectory must supply rules for building sources it contributes
src/tap-bridge/examples/%.o: ../src/tap-bridge/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


