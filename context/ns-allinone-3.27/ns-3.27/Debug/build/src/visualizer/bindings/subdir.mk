################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../build/src/visualizer/bindings/ns3module.cc.7.o 

CC_SRCS += \
../build/src/visualizer/bindings/ns3module.cc 

OBJS += \
./build/src/visualizer/bindings/ns3module.o 

CC_DEPS += \
./build/src/visualizer/bindings/ns3module.d 


# Each subdirectory must supply rules for building sources it contributes
build/src/visualizer/bindings/%.o: ../build/src/visualizer/bindings/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

