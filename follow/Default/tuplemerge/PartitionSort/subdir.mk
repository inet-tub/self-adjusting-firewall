################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tuplemerge/PartitionSort/DISCPAC.cpp \
../tuplemerge/PartitionSort/IntervalTree.cpp \
../tuplemerge/PartitionSort/LongestIncreasingSubsequence.cpp \
../tuplemerge/PartitionSort/MITree.cpp \
../tuplemerge/PartitionSort/OptimizedMITree.cpp \
../tuplemerge/PartitionSort/PartitionSort.cpp \
../tuplemerge/PartitionSort/RuleSplitter.cpp \
../tuplemerge/PartitionSort/SortableRulesetPartitioner.cpp \
../tuplemerge/PartitionSort/misc.cpp \
../tuplemerge/PartitionSort/red_black_tree.cpp \
../tuplemerge/PartitionSort/stack.cpp \
../tuplemerge/PartitionSort/test_red_black_tree.cpp 

OBJS += \
./tuplemerge/PartitionSort/DISCPAC.o \
./tuplemerge/PartitionSort/IntervalTree.o \
./tuplemerge/PartitionSort/LongestIncreasingSubsequence.o \
./tuplemerge/PartitionSort/MITree.o \
./tuplemerge/PartitionSort/OptimizedMITree.o \
./tuplemerge/PartitionSort/PartitionSort.o \
./tuplemerge/PartitionSort/RuleSplitter.o \
./tuplemerge/PartitionSort/SortableRulesetPartitioner.o \
./tuplemerge/PartitionSort/misc.o \
./tuplemerge/PartitionSort/red_black_tree.o \
./tuplemerge/PartitionSort/stack.o \
./tuplemerge/PartitionSort/test_red_black_tree.o 

CPP_DEPS += \
./tuplemerge/PartitionSort/DISCPAC.d \
./tuplemerge/PartitionSort/IntervalTree.d \
./tuplemerge/PartitionSort/LongestIncreasingSubsequence.d \
./tuplemerge/PartitionSort/MITree.d \
./tuplemerge/PartitionSort/OptimizedMITree.d \
./tuplemerge/PartitionSort/PartitionSort.d \
./tuplemerge/PartitionSort/RuleSplitter.d \
./tuplemerge/PartitionSort/SortableRulesetPartitioner.d \
./tuplemerge/PartitionSort/misc.d \
./tuplemerge/PartitionSort/red_black_tree.d \
./tuplemerge/PartitionSort/stack.d \
./tuplemerge/PartitionSort/test_red_black_tree.d 


# Each subdirectory must supply rules for building sources it contributes
tuplemerge/PartitionSort/%.o: ../tuplemerge/PartitionSort/%.cpp tuplemerge/PartitionSort/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/vamsi/src/phd/self-adjusting-firewall/follow/tuplemerge -O2 -g -pedantic -w -c -fmessage-length=0 -fpermissive -fopenmp -O3 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


