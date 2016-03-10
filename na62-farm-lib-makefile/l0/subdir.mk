################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/l0/MEP.cpp \
../na62-farm-lib/l0/MEPFragment.cpp \
../na62-farm-lib/l0/Subevent.cpp 

OBJS += \
./l0/MEP.o \
./l0/MEPFragment.o \
./l0/Subevent.o 

CPP_DEPS += \
./l0/MEP.d \
./l0/MEPFragment.d \
./l0/Subevent.d 


# Each subdirectory must supply rules for building sources it contributes
l0/%.o: ../na62-farm-lib/l0/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


