################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/sixlowpan/examples/example-ping-lr-wpan.cc \
../src/sixlowpan/examples/example-sixlowpan.cc 

OBJS += \
./src/sixlowpan/examples/example-ping-lr-wpan.o \
./src/sixlowpan/examples/example-sixlowpan.o 

CC_DEPS += \
./src/sixlowpan/examples/example-ping-lr-wpan.d \
./src/sixlowpan/examples/example-sixlowpan.d 


# Each subdirectory must supply rules for building sources it contributes
src/sixlowpan/examples/%.o: ../src/sixlowpan/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


