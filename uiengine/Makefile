# Makefile for linedriver

ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = uiengine

LIBELVUTILSPATH = ../libs/elvutils
LIBELVUTILS = elvutils

LIBXMLPARSERPATH = ../libs/xmlparser
LIBXMLPARSER = xmlparser

DIRECTFBINCLUDEPATH = $(shell dirname `which arm-linux-g++`)/../include/directfb

XMLRPCPATH = ../libs/xmlrpc
XMLRPCLIB = libXmlRpc.so
XMLRPCINCLUDE = $(XMLRPCPATH)/src

TCLPATH = ../tcl
TCLVERSION = $(shell awk -F "[ \t]*=[ \t]*" 'NF==2 && $$1 == "VERSION" {print $$2}' $(TCLPATH)/unix/Makefile)
TCLINCLUDEPATH = $(TCLPATH)/generic
TCLLIBPATH = $(TCLPATH)/unix
TCLLIB = -L$(TCLLIBPATH) -ltcl$(TCLVERSION)

TARGETOBJS = $(TARGET).o \
				dynamic.o \
				InputManager.o \
				UiPage.o \
				UiElement.o \
				UiImage.o \
				UiText.o \
				UiManager.o \
				UiDateTime.o \
				UiBacklight.o \
				PageScriptingEngine.o \
				IcClient.o \
				CachedImage.o \
				AudioManager.o \
				AudioPlayer.o \
				AudioPlaylist.o \
				AudioFileInfo.o \
				AlsaMixer.o

CC = arm-linux-g++
CXX = arm-linux-g++
CXXFLAGS +=  -O2 -I$(DIRECTFBINCLUDEPATH) -I$(LIBELVUTILSPATH) -I$(LIBXMLPARSERPATH) -I$(XMLRPCINCLUDE) -I$(TCLINCLUDEPATH) -Wall 
LDFLAGS += -lpthread -ldirectfb -lasound -L$(LIBELVUTILSPATH) -l$(LIBELVUTILS) -L$(LIBXMLPARSERPATH) -l$(LIBXMLPARSER) -L$(XMLRPCPATH) -lXmlRpc $(TCLLIB)

all: $(TARGET)

$(TARGET): $(TARGETOBJS)

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
