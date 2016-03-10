################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../na62-farm-lib/eventBuilding/Event.cpp \
../na62-farm-lib/eventBuilding/EventPool.cpp \
../na62-farm-lib/eventBuilding/SourceIDManager.cpp \
../na62-farm-lib/eventBuilding/UnfinishedEventsCollector.cpp 

OBJS += \
./eventBuilding/Event.o \
./eventBuilding/EventPool.o \
./eventBuilding/SourceIDManager.o \
./eventBuilding/UnfinishedEventsCollector.o 

CPP_DEPS += \
./eventBuilding/Event.d \
./eventBuilding/EventPool.d \
./eventBuilding/SourceIDManager.d \
./eventBuilding/UnfinishedEventsCollector.d 


# Each subdirectory must supply rules for building sources it contributes
eventBuilding/%.o: ../na62-farm-lib/eventBuilding/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -c -fmessage-length=0 -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


