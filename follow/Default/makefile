################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm

# All of the sources participating in the build are defined here
-include sources.mk
-include tuplemerge/Utilities/subdir.mk
-include tuplemerge/TupleMerge/subdir.mk
-include tuplemerge/PartitionSort/subdir.mk
-include tuplemerge/OVS/subdir.mk
-include tuplemerge/IO/subdir.mk
-include tuplemerge/ClassBenchTraceGenerator/subdir.mk
-include tuplemerge/subdir.mk
-include src/follow/subdir.mk
-include src/cuts/subdir.mk
-include src/cmd/subdir.mk
-include src/classifier/subdir.mk
-include src/Tuplemerge/subdir.mk
-include examples/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: follow

# Tool invocations
follow: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "follow" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) follow
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
