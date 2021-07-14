################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/BitVector/BitSet.cpp \
../tuplemerge/BitVector/BitVector.cpp \
../tuplemerge/BitVector/EqnMatcher.cpp \
../tuplemerge/BitVector/LongestPrefixMatch.cpp 

OBJS += \
./tuplemerge/BitVector/BitSet.o \
./tuplemerge/BitVector/BitVector.o \
./tuplemerge/BitVector/EqnMatcher.o \
./tuplemerge/BitVector/LongestPrefixMatch.o 

CPP_DEPS += \
./tuplemerge/BitVector/BitSet.d \
./tuplemerge/BitVector/BitVector.d \
./tuplemerge/BitVector/EqnMatcher.d \
./tuplemerge/BitVector/LongestPrefixMatch.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/BitVector/%.o: ../tuplemerge/BitVector/%.cpp tuplemerge/BitVector/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/vamsi/src/phd/self-adjusting-firewall/follow/tuplemerge -I/home/vamsi/src/phd/self-adjusting-firewall/follow/includes -O2 -g -pedantic -w -c -fmessage-length=0 -fpermissive -fopenmp -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


