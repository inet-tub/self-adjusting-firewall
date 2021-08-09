# TARGET_EXEC ?= simulator

# BUILD_DIR ?= ./build
# SRC_DIRS ?= ./src ./examples

# SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
# OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
# DEPS := $(OBJS:.o=.d)

# INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -g -w -std=c++11 -fpermissive -O3 

# $(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
# 	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# # assembly
# $(BUILD_DIR)/%.s.o: %.s
# 	$(MKDIR_P) $(dir $@)
# 	$(AS) $(ASFLAGS) -c $< -o $@

# # c source
# $(BUILD_DIR)/%.c.o: %.c
# 	$(MKDIR_P) $(dir $@)
# 	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# # c++ source
# $(BUILD_DIR)/%.cpp.o: %.cpp
# 	$(MKDIR_P) $(dir $@)
# 	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


# .PHONY: clean

# clean:
# 	$(RM) -r $(BUILD_DIR)

# -include $(DEPS)

# MKDIR_P ?= mkdir -p


CPP=g++
CFLAGS = -g -w -std=c++11 -fpermissive -O3 $(INCLUDE)
BUILDDIR := $(shell pwd)/build
SRC_DIRS ?= $(shell pwd)/src $(shell pwd)/examples
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)

classify: $(SRCS)
	${CPP} ${CFLAGS} -o $(BUILDDIR)/classify $(SRCS)

clean:
	rm -r build/*
