################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/openflow/model/openflow-interface.cc \
../src/openflow/model/openflow-switch-net-device.cc 

OBJS += \
./src/openflow/model/openflow-interface.o \
./src/openflow/model/openflow-switch-net-device.o 

CC_DEPS += \
./src/openflow/model/openflow-interface.d \
./src/openflow/model/openflow-switch-net-device.d 


# Each subdirectory must supply rules for building sources it contributes
src/openflow/model/%.o: ../src/openflow/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


