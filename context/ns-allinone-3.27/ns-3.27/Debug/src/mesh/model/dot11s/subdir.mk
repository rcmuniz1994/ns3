################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/mesh/model/dot11s/airtime-metric.cc \
../src/mesh/model/dot11s/dot11s-mac-header.cc \
../src/mesh/model/dot11s/hwmp-protocol-mac.cc \
../src/mesh/model/dot11s/hwmp-protocol.cc \
../src/mesh/model/dot11s/hwmp-rtable.cc \
../src/mesh/model/dot11s/hwmp-tag.cc \
../src/mesh/model/dot11s/ie-dot11s-beacon-timing.cc \
../src/mesh/model/dot11s/ie-dot11s-configuration.cc \
../src/mesh/model/dot11s/ie-dot11s-id.cc \
../src/mesh/model/dot11s/ie-dot11s-metric-report.cc \
../src/mesh/model/dot11s/ie-dot11s-peer-management.cc \
../src/mesh/model/dot11s/ie-dot11s-peering-protocol.cc \
../src/mesh/model/dot11s/ie-dot11s-perr.cc \
../src/mesh/model/dot11s/ie-dot11s-prep.cc \
../src/mesh/model/dot11s/ie-dot11s-preq.cc \
../src/mesh/model/dot11s/ie-dot11s-rann.cc \
../src/mesh/model/dot11s/peer-link-frame.cc \
../src/mesh/model/dot11s/peer-link.cc \
../src/mesh/model/dot11s/peer-management-protocol-mac.cc \
../src/mesh/model/dot11s/peer-management-protocol.cc 

OBJS += \
./src/mesh/model/dot11s/airtime-metric.o \
./src/mesh/model/dot11s/dot11s-mac-header.o \
./src/mesh/model/dot11s/hwmp-protocol-mac.o \
./src/mesh/model/dot11s/hwmp-protocol.o \
./src/mesh/model/dot11s/hwmp-rtable.o \
./src/mesh/model/dot11s/hwmp-tag.o \
./src/mesh/model/dot11s/ie-dot11s-beacon-timing.o \
./src/mesh/model/dot11s/ie-dot11s-configuration.o \
./src/mesh/model/dot11s/ie-dot11s-id.o \
./src/mesh/model/dot11s/ie-dot11s-metric-report.o \
./src/mesh/model/dot11s/ie-dot11s-peer-management.o \
./src/mesh/model/dot11s/ie-dot11s-peering-protocol.o \
./src/mesh/model/dot11s/ie-dot11s-perr.o \
./src/mesh/model/dot11s/ie-dot11s-prep.o \
./src/mesh/model/dot11s/ie-dot11s-preq.o \
./src/mesh/model/dot11s/ie-dot11s-rann.o \
./src/mesh/model/dot11s/peer-link-frame.o \
./src/mesh/model/dot11s/peer-link.o \
./src/mesh/model/dot11s/peer-management-protocol-mac.o \
./src/mesh/model/dot11s/peer-management-protocol.o 

CC_DEPS += \
./src/mesh/model/dot11s/airtime-metric.d \
./src/mesh/model/dot11s/dot11s-mac-header.d \
./src/mesh/model/dot11s/hwmp-protocol-mac.d \
./src/mesh/model/dot11s/hwmp-protocol.d \
./src/mesh/model/dot11s/hwmp-rtable.d \
./src/mesh/model/dot11s/hwmp-tag.d \
./src/mesh/model/dot11s/ie-dot11s-beacon-timing.d \
./src/mesh/model/dot11s/ie-dot11s-configuration.d \
./src/mesh/model/dot11s/ie-dot11s-id.d \
./src/mesh/model/dot11s/ie-dot11s-metric-report.d \
./src/mesh/model/dot11s/ie-dot11s-peer-management.d \
./src/mesh/model/dot11s/ie-dot11s-peering-protocol.d \
./src/mesh/model/dot11s/ie-dot11s-perr.d \
./src/mesh/model/dot11s/ie-dot11s-prep.d \
./src/mesh/model/dot11s/ie-dot11s-preq.d \
./src/mesh/model/dot11s/ie-dot11s-rann.d \
./src/mesh/model/dot11s/peer-link-frame.d \
./src/mesh/model/dot11s/peer-link.d \
./src/mesh/model/dot11s/peer-management-protocol-mac.d \
./src/mesh/model/dot11s/peer-management-protocol.d 


# Each subdirectory must supply rules for building sources it contributes
src/mesh/model/dot11s/%.o: ../src/mesh/model/dot11s/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


