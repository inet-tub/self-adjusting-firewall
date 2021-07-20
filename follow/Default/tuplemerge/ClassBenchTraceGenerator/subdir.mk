################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../tuplemerge/ClassBenchTraceGenerator/trace_tools.cc 

CC_DEPS += \
./tuplemerge/ClassBenchTraceGenerator/trace_tools.d 

OBJS += \
./tuplemerge/ClassBenchTraceGenerator/trace_tools.o 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/ClassBenchTraceGenerator/%.o: ../tuplemerge/ClassBenchTraceGenerator/%.cc tuplemerge/ClassBenchTraceGenerator/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIE -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


