################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/lr-wpan/test/lr-wpan-ack-test.cc \
../src/lr-wpan/test/lr-wpan-cca-test.cc \
../src/lr-wpan/test/lr-wpan-collision-test.cc \
../src/lr-wpan/test/lr-wpan-ed-test.cc \
../src/lr-wpan/test/lr-wpan-error-model-test.cc \
../src/lr-wpan/test/lr-wpan-packet-test.cc \
../src/lr-wpan/test/lr-wpan-pd-plme-sap-test.cc \
../src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.cc 

OBJS += \
./src/lr-wpan/test/lr-wpan-ack-test.o \
./src/lr-wpan/test/lr-wpan-cca-test.o \
./src/lr-wpan/test/lr-wpan-collision-test.o \
./src/lr-wpan/test/lr-wpan-ed-test.o \
./src/lr-wpan/test/lr-wpan-error-model-test.o \
./src/lr-wpan/test/lr-wpan-packet-test.o \
./src/lr-wpan/test/lr-wpan-pd-plme-sap-test.o \
./src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.o 

CC_DEPS += \
./src/lr-wpan/test/lr-wpan-ack-test.d \
./src/lr-wpan/test/lr-wpan-cca-test.d \
./src/lr-wpan/test/lr-wpan-collision-test.d \
./src/lr-wpan/test/lr-wpan-ed-test.d \
./src/lr-wpan/test/lr-wpan-error-model-test.d \
./src/lr-wpan/test/lr-wpan-packet-test.d \
./src/lr-wpan/test/lr-wpan-pd-plme-sap-test.d \
./src/lr-wpan/test/lr-wpan-spectrum-value-helper-test.d 


# Each subdirectory must supply rules for building sources it contributes
src/lr-wpan/test/%.o: ../src/lr-wpan/test/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


