################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/CutSplit/CutSplit.cpp \
../src/CutSplit/hs.cpp \
../src/CutSplit/trie.cpp 

OBJS += \
./src/CutSplit/CutSplit.o \
./src/CutSplit/hs.o \
./src/CutSplit/trie.o 

CPP_DEPS += \
./src/CutSplit/CutSplit.d \
./src/CutSplit/hs.d \
./src/CutSplit/trie.d 


# Each subdirectory must supply rules for building sources it contributes
src/CutSplit/%.o: ../src/CutSplit/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -g -pedantic -w -O3 -g -pedantic -w -c -fmessage-length=0 -fpermissive  -O3 -Wall -fPIC -MMD -MP -MF -fPIE -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


