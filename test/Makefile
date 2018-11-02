CXX := g++

CURRENT_DIR=$(shell echo `pwd`)
PARENT_DIRNAME=$(shell basename $(shell dirname $(CURRENT_DIR)))
SOURCE_DIR=$(CURRENT_DIR)

GTESTDIR = ../3rdparty/gtest

CPPFLAGS= -g -c -fPIC -D_DEBUG \
		  -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings \
		  -Wsign-compare -Winvalid-pch -fms-extensions -Wall \
		  -MMD -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords -std=c++11\
		  -I ./$(GTESTDIR) \
		  -I .. \
		  -I $(SOURCE_DIR)/../3rdparty \

RTFLAGS := \
    -Wl,-rpath=. \
    -Wl,-rpath=$(GCC472_PATH)/lib64 \
    -Wl,-rpath=$(CURRENT_DIR)/../simcc

LDFLAGS= $(RTFLAGS) \
	 -L$(CURRENT_DIR)/../simcc \
	 -lsimcc \
	-lpthread

DIRNAME=$(shell basename `pwd`)

SRCS := $(wildcard *.cc) $(GTESTDIR)/src/gtest-all.cc $(GTESTDIR)/src/gtest_main.cc
OBJS := $(patsubst %.cc, %.o, $(SRCS))
DEPS := $(patsubst %.o, %.d, $(OBJS))

TARGET=$(PARENT_DIRNAME)

all : $(TARGET) 

check : $(TARGET)
	./$^

$(TARGET) : $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

-include $(DEPS)

%.o : %.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	rm -rf *.o *.d $(OBJS) $(DEPS) $(TARGET) *.tmp decoded.txt log

