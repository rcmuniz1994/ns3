################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/visualizer/model/dummy-file-for-static-builds.cc \
../src/visualizer/model/pyviz.cc \
../src/visualizer/model/visual-simulator-impl.cc 

OBJS += \
./src/visualizer/model/dummy-file-for-static-builds.o \
./src/visualizer/model/pyviz.o \
./src/visualizer/model/visual-simulator-impl.o 

CC_DEPS += \
./src/visualizer/model/dummy-file-for-static-builds.d \
./src/visualizer/model/pyviz.d \
./src/visualizer/model/visual-simulator-impl.d 


# Each subdirectory must supply rules for building sources it contributes
src/visualizer/model/%.o: ../src/visualizer/model/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


