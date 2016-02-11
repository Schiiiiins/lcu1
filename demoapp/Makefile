# Makefile for linedriver

ROMFSINST = ../helper_scripts/romfs-inst.sh

TARGET = demoapp

DIRECTFBINCLUDEPATH = $(shell dirname `which arm-linux-g++`)/../include/directfb

TARGETOBJS = $(TARGET).o \

CC = arm-linux-g++
CXX = arm-linux-g++
CXXFLAGS +=  -O2 -I$(DIRECTFBINCLUDEPATH) -Wall 
LDFLAGS += -lpthread -ldirectfb

all: $(TARGET)

$(TARGET): $(TARGETOBJS)

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)
	mkdir -p $(ROMFSDIR)/usr/share/fonts
	$(ROMFSINST) /usr/share/fonts/decker.ttf

clean:
	rm -rf *.o
	rm -rf $(TARGET)
