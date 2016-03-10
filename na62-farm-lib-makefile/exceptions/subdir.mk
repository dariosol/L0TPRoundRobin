################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/exceptions/NA62Error.cpp 

OBJS += \
./exceptions/NA62Error.o 

CPP_DEPS += \
./exceptions/NA62Error.d 


# Each subdirectory must supply rules for building sources it contributes
exceptions/%.o: ../na62-farm-lib/exceptions/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


