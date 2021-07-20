################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cmd/CommandLine.cpp 

OBJS += \
./src/cmd/CommandLine.o 

CPP_DEPS += \
./src/cmd/CommandLine.d 


# Each subdirectory must supply rules for building sources it contributes
src/cmd/%.o: ../src/cmd/%.cpp src/cmd/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIE -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


