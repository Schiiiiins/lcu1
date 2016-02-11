# Makefile for xmlparser

ROMFSINST = $(shell if [ -x ../../helper_scripts/romfs-inst.sh ]; then echo ../../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = libxmlparser

CROSS = arm-linux-

TARGETOBJS = xmlParser.o

ifeq "$(DEBUG)" "1"
OPTIMIZE = -g -O0
else
OPTIMIZE = -O2
endif

CC = $(CROSS)g++
CXX = $(CROSS)g++
LD = $(CROSS)g++
AR = $(CROSS)ar
STRIP = $(CROSS)strip
CXXFLAGS +=  $(OPTIMIZE) -Wall -fPIC
LDFLAGS += -shared -fPIC

all: $(TARGET).so $(TARGET).a

$(TARGET).so: $(TARGETOBJS)
	$(LD) $(LDFLAGS) -o $(TARGET).so $(TARGETOBJS)
	
$(TARGET).a: $(TARGETOBJS)
	$(AR) rcs $(TARGET).a $(TARGETOBJS)

install: $(TARGET).so
	$(STRIP) $(TARGET).so
	$(ROMFSINST) /lib/$(TARGET).so


clean:
	rm -rf *.o
	rm -rf $(TARGET).*
