################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/Utilities/EffectiveGrid.cpp \
../tuplemerge/Utilities/IntervalUtilities.cpp \
../tuplemerge/Utilities/MapExtensions.cpp \
../tuplemerge/Utilities/Tcam.cpp 

OBJS += \
./tuplemerge/Utilities/EffectiveGrid.o \
./tuplemerge/Utilities/IntervalUtilities.o \
./tuplemerge/Utilities/MapExtensions.o \
./tuplemerge/Utilities/Tcam.o 

CPP_DEPS += \
./tuplemerge/Utilities/EffectiveGrid.d \
./tuplemerge/Utilities/IntervalUtilities.d \
./tuplemerge/Utilities/MapExtensions.d \
./tuplemerge/Utilities/Tcam.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/Utilities/%.o: ../tuplemerge/Utilities/%.cpp tuplemerge/Utilities/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIE -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


