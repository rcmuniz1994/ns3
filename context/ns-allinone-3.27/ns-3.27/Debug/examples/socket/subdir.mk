################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../examples/socket/socket-bound-static-routing.cc \
../examples/socket/socket-bound-tcp-static-routing.cc \
../examples/socket/socket-options-ipv4.cc \
../examples/socket/socket-options-ipv6.cc 

OBJS += \
./examples/socket/socket-bound-static-routing.o \
./examples/socket/socket-bound-tcp-static-routing.o \
./examples/socket/socket-options-ipv4.o \
./examples/socket/socket-options-ipv6.o 

CC_DEPS += \
./examples/socket/socket-bound-static-routing.d \
./examples/socket/socket-bound-tcp-static-routing.d \
./examples/socket/socket-options-ipv4.d \
./examples/socket/socket-options-ipv6.d 


# Each subdirectory must supply rules for building sources it contributes
examples/socket/%.o: ../examples/socket/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


