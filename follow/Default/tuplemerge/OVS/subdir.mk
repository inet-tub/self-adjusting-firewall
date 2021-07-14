################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/OVS/TupleSpaceSearch.cpp \
../tuplemerge/OVS/cmap.cpp 

OBJS += \
./tuplemerge/OVS/TupleSpaceSearch.o \
./tuplemerge/OVS/cmap.o 

CPP_DEPS += \
./tuplemerge/OVS/TupleSpaceSearch.d \
./tuplemerge/OVS/cmap.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/OVS/%.o: ../tuplemerge/OVS/%.cpp tuplemerge/OVS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -pedantic -w -c -fmessage-length=0 -fpermissive -fopenmp -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


