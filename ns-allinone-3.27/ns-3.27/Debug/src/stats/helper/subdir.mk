################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/stats/helper/file-helper.cc \
../src/stats/helper/gnuplot-helper.cc 

OBJS += \
./src/stats/helper/file-helper.o \
./src/stats/helper/gnuplot-helper.o 

CC_DEPS += \
./src/stats/helper/file-helper.d \
./src/stats/helper/gnuplot-helper.d 


# Each subdirectory must supply rules for building sources it contributes
src/stats/helper/%.o: ../src/stats/helper/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


