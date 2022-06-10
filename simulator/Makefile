CPP=g++
CFLAGS = -g -w -std=c++11 -fpermissive -O3 $(INCLUDE)
BUILDDIR := $(shell pwd)/tools
SRC_DIRS ?= $(shell pwd)/src $(shell pwd)/examples
SRCS := $(shell find $(SRC_DIRS) -name *.cpp)

classify: $(SRCS)
	${CPP} ${CFLAGS} -o $(BUILDDIR)/classify $(SRCS)

clean:
	rm -r ${BUILDDIR}/*
