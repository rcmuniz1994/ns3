################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/internet-apps/test/dhcp-test.cc 

OBJS += \
./src/internet-apps/test/dhcp-test.o 

CC_DEPS += \
./src/internet-apps/test/dhcp-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/internet-apps/test/%.o: ../src/internet-apps/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


