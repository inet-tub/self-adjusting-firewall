################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/classifier/ClassifierHelper.cpp 

OBJS += \
./src/classifier/ClassifierHelper.o 

CPP_DEPS += \
./src/classifier/ClassifierHelper.d 


# Each subdirectory must supply rules for building sources it contributes
src/classifier/%.o: ../src/classifier/%.cpp src/classifier/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -pedantic -w -c -fmessage-length=0 -fpermissive -fopenmp -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


