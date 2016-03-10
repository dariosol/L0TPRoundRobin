################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/LKr/LkrFragment.cpp 

OBJS += \
./LKr/LkrFragment.o 

CPP_DEPS += \
./LKr/LkrFragment.d 


# Each subdirectory must supply rules for building sources it contributes
LKr/%.o: ../na62-farm-lib/LKr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


