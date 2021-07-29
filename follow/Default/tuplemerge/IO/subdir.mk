################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/IO/InputReader.cpp \
../tuplemerge/IO/OutputWriter.cpp 

OBJS += \
./tuplemerge/IO/InputReader.o \
./tuplemerge/IO/OutputWriter.o 

CPP_DEPS += \
./tuplemerge/IO/InputReader.d \
./tuplemerge/IO/OutputWriter.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/IO/%.o: ../tuplemerge/IO/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


