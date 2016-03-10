################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/utils/AExecutable.cpp \
../na62-farm-lib/utils/DataDumper.cpp \
../na62-farm-lib/utils/Stopwatch.cpp \
../na62-farm-lib/utils/Utils.cpp 

OBJS += \
./utils/AExecutable.o \
./utils/DataDumper.o \
./utils/Stopwatch.o \
./utils/Utils.o 

CPP_DEPS += \
./utils/AExecutable.d \
./utils/DataDumper.d \
./utils/Stopwatch.d \
./utils/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../na62-farm-lib/utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


