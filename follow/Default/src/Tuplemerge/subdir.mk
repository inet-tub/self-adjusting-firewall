################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Tuplemerge/Tuplemerge.cpp 

OBJS += \
./src/Tuplemerge/Tuplemerge.o 

CPP_DEPS += \
./src/Tuplemerge/Tuplemerge.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tuplemerge/%.o: ../src/Tuplemerge/%.cpp src/Tuplemerge/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/vamsi/src/phd/self-adjusting-firewall/follow/tuplemerge -O2 -g -pedantic -w -c -fmessage-length=0 -fpermissive -fopenmp -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


