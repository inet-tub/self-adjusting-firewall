################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/TupleMerge/SlottedTable.cpp \
../tuplemerge/TupleMerge/TupleMergeOffline.cpp \
../tuplemerge/TupleMerge/TupleMergeOnline.cpp 

OBJS += \
./tuplemerge/TupleMerge/SlottedTable.o \
./tuplemerge/TupleMerge/TupleMergeOffline.o \
./tuplemerge/TupleMerge/TupleMergeOnline.o 

CPP_DEPS += \
./tuplemerge/TupleMerge/SlottedTable.d \
./tuplemerge/TupleMerge/TupleMergeOffline.d \
./tuplemerge/TupleMerge/TupleMergeOnline.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/TupleMerge/%.o: ../tuplemerge/TupleMerge/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


