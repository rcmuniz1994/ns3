################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/nix-vector-routing/examples/nix-simple.cc \
../src/nix-vector-routing/examples/nms-p2p-nix.cc 

OBJS += \
./src/nix-vector-routing/examples/nix-simple.o \
./src/nix-vector-routing/examples/nms-p2p-nix.o 

CC_DEPS += \
./src/nix-vector-routing/examples/nix-simple.d \
./src/nix-vector-routing/examples/nms-p2p-nix.d 


# Each subdirectory must supply rules for building sources it contributes
src/nix-vector-routing/examples/%.o: ../src/nix-vector-routing/examples/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


