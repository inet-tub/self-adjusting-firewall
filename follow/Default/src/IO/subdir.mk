################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/IO/InputReader.cpp 

OBJS += \
./src/IO/InputReader.o 

CPP_DEPS += \
./src/IO/InputReader.d 


# Each subdirectory must supply rules for building sources it contributes
src/IO/%.o: ../src/IO/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g -pedantic -w -O3 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIC -MMD -MP -MF -fPIE -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


