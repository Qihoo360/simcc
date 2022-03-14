include ../rpm/build.mk
CC=gcc
CXX=g++
AR=ar
ARFLAGS=cru
CURRENT_DIR=$(shell echo `pwd`)
SOURCE_DIR=$(CURRENT_DIR)
CFLAGS=$(TOP_CFLAGS) $(DEBUG) -g -c -fPIC \
	   -O3 -Wall -Wno-unused-parameter -Wno-unused-function -Wunused-variable -Wunused-value \
	   -Wshadow -Wcast-qual -Wcast-align -Wwrite-strings -Wsign-compare \
	   -Winvalid-pch -fms-extensions -Wfloat-equal -Wextra -std=c++11 -DGOOGLE_STRIP_LOG=0 \
	   -I .. -I ../include -MMD
CPPFLAGS= $(CFLAGS) -Woverloaded-virtual -Wsign-promo -fno-gnu-keywords

RTFLAGS := \
    -Wl,-rpath=.

LDFLAGS= $(RTFLAGS) -lpthread \
		 -ldl


SRCS := $(wildcard *.cc) $(wildcard json/*.cc) $(wildcard misc/*.cc)
OBJS := $(patsubst %.cc, %.o, $(SRCS)) $(CCOBJS)
DEPS := $(patsubst %.o, %.d, $(OBJS))

LIBNAME=simcc
TARGET_SO=lib$(LIBNAME).so
TARGET_A=lib$(LIBNAME).a

all : $(TARGET_SO) $(TARGET_A)
	$(MAKE) install

$(TARGET_A) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS) 
	ranlib $@			   

$(TARGET_SO) : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -shared -o $@.$(RPM_VERSION)
	@rm -fr $(@)
	@rm -fr $(@).$(MAJOR)
	ln -s $(@).$(RPM_VERSION) $(@).$(MAJOR)
	ln -s $(@).$(MAJOR) $(@)
	
	
%.o : %.cc 
	$(CXX) $(CPPFLAGS) $< -o $@

-include $(DEPS)

t:
	@echo "LIBNAME:" $(LIBNAME)
	@echo "SRCS:" $(SRCS)
	@echo "OBJS:" $(OBJS)
	@echo "CFLAGS:" $(CFLAGS)
	@echo "CPPFLAGS:" $(CPPFLAGS)

install: $(TARGET_A) $(TARGET_SO) 

clean:
	rm -rf $(OBJS) $(DEPS) $(TARGET_SO)* $(TARGET_A)

