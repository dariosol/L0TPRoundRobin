################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/monitoring/BurstIdHandler.cpp \
../na62-farm-lib/monitoring/FarmStatistics.cpp 

OBJS += \
./monitoring/BurstIdHandler.o \
./monitoring/FarmStatistics.o 

CPP_DEPS += \
./monitoring/BurstIdHandler.d \
./monitoring/FarmStatistics.d 


# Each subdirectory must supply rules for building sources it contributes
monitoring/%.o: ../na62-farm-lib/monitoring/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


