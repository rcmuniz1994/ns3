################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/openflow/test/openflow-switch-test-suite.cc 

OBJS += \
./src/openflow/test/openflow-switch-test-suite.o 

CC_DEPS += \
./src/openflow/test/openflow-switch-test-suite.d 


# Each subdirectory must supply rules for building sources it contributes
src/openflow/test/%.o: ../src/openflow/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


