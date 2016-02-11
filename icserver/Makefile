# Makefile for rfd

ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = icserver

LIBELVUTILSPATH = ../libs/elvutils
LIBELVUTILS = elvutils

LIBXMLPARSERPATH = ../libs/xmlparser
LIBXMLPARSER = xmlparser

XMLRPCPATH = ../libs/xmlrpc
XMLRPCLIB = libXmlRpc.so
XMLRPCINCLUDE = $(XMLRPCPATH)/src

TARGETOBJS = $(TARGET).o \
	AsyncXmlRpcSender.o \
	HmConfigParameter.o \
	HmInterface.o \
	IcChannel.o \
	IcSubChannel.o \
	IcMainChannel.o \
	Manager.o \
	Connection.o \
	HmManager.o \
	IcClientConnection.o \
	Socket.o \
	HmChannel.o \
	HmValueConnection.o \
	IcReceiver.o \
	XmlRpcMethods.o \
	HmDevice.o \
	HmValue.o \
	XmlLoop.o \
	ExpressionParser.o \


CC = arm-linux-g++
CXX = arm-linux-g++
CXXFLAGS +=  -O2 -I$(LIBELVUTILSPATH) -I$(LIBXMLPARSERPATH) -I$(XMLRPCINCLUDE) -Wall 
LDFLAGS += -lpthread -L$(LIBELVUTILSPATH) -l$(LIBELVUTILS) -L$(LIBXMLPARSERPATH) -l$(LIBXMLPARSER) -L$(XMLRPCPATH) -lXmlRpc $(TCLLIB)

all: $(TARGET)

$(TARGET): $(TARGETOBJS)

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)


clean:
	rm -rf *.o
	rm -rf $(TARGET)
