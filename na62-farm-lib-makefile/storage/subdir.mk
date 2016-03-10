################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/storage/BurstFileWriter.cpp \
../na62-farm-lib/storage/EventSerializer.cpp 

OBJS += \
./storage/BurstFileWriter.o \
./storage/EventSerializer.o 

CPP_DEPS += \
./storage/BurstFileWriter.d \
./storage/EventSerializer.d 


# Each subdirectory must supply rules for building sources it contributes
storage/%.o: ../na62-farm-lib/storage/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


