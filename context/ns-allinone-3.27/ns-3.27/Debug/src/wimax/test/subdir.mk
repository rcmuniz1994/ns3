################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/wimax/test/mac-messages-test.cc \
../src/wimax/test/phy-test.cc \
../src/wimax/test/qos-test.cc \
../src/wimax/test/ss-mac-test.cc \
../src/wimax/test/wimax-fragmentation-test.cc \
../src/wimax/test/wimax-service-flow-test.cc \
../src/wimax/test/wimax-tlv-test.cc 

OBJS += \
./src/wimax/test/mac-messages-test.o \
./src/wimax/test/phy-test.o \
./src/wimax/test/qos-test.o \
./src/wimax/test/ss-mac-test.o \
./src/wimax/test/wimax-fragmentation-test.o \
./src/wimax/test/wimax-service-flow-test.o \
./src/wimax/test/wimax-tlv-test.o 

CC_DEPS += \
./src/wimax/test/mac-messages-test.d \
./src/wimax/test/phy-test.d \
./src/wimax/test/qos-test.d \
./src/wimax/test/ss-mac-test.d \
./src/wimax/test/wimax-fragmentation-test.d \
./src/wimax/test/wimax-service-flow-test.d \
./src/wimax/test/wimax-tlv-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/wimax/test/%.o: ../src/wimax/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


