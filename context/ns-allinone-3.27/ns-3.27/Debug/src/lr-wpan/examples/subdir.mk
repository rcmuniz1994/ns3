################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/lr-wpan/examples/lr-wpan-data.cc \
../src/lr-wpan/examples/lr-wpan-error-distance-plot.cc \
../src/lr-wpan/examples/lr-wpan-error-model-plot.cc \
../src/lr-wpan/examples/lr-wpan-packet-print.cc \
../src/lr-wpan/examples/lr-wpan-phy-test.cc 

OBJS += \
./src/lr-wpan/examples/lr-wpan-data.o \
./src/lr-wpan/examples/lr-wpan-error-distance-plot.o \
./src/lr-wpan/examples/lr-wpan-error-model-plot.o \
./src/lr-wpan/examples/lr-wpan-packet-print.o \
./src/lr-wpan/examples/lr-wpan-phy-test.o 

CC_DEPS += \
./src/lr-wpan/examples/lr-wpan-data.d \
./src/lr-wpan/examples/lr-wpan-error-distance-plot.d \
./src/lr-wpan/examples/lr-wpan-error-model-plot.d \
./src/lr-wpan/examples/lr-wpan-packet-print.d \
./src/lr-wpan/examples/lr-wpan-phy-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/lr-wpan/examples/%.o: ../src/lr-wpan/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


