# Makefile for tclrpc extension

ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)
STRIP = arm-linux-strip

TCLRPC = tclrpc.so

TCLPATH = ../tcl
TCLVERSION = $(shell awk -F "[ \t]*=[ \t]*" 'NF==2 && $$1 == "VERSION" {print $$2}' $(TCLPATH)/unix/Makefile)
TCLINCLUDEPATH = $(TCLPATH)/generic
TCLLIBPATH = $(TCLPATH)/unix
TCLLIB = -L$(TCLLIBPATH) -ltcl$(TCLVERSION)

XMLRPCPATH = ../libs/xmlrpc
XMLRPCLIB = libXmlRpc.so
#XMLRPCOBJS = $(XMLRPCPATH)/$(XMLRPCLIB)
XMLRPCINCLUDE = $(XMLRPCPATH)/src

TCLRPCOBJS = tclrpc.o

CC = arm-linux-g++
CXX = arm-linux-g++
LD = arm-linux-g++
CXXFLAGS +=  -fPIC -O2 -I$(XMLRPCINCLUDE) -I$(TCLINCLUDEPATH) -Wall 
LDFLAGS += -shared -fPIC $(TCLLIB) -lstdc++ -L$(XMLRPCPATH) -lXmlRpc

all: xmlrpc $(TCLRPC)

xmlrpc_clean:
	make -C $(XMLRPCPATH) clean
	 
xmlrpc:
	make -C $(XMLRPCPATH) $(XMLRPCLIB)
	 
xmlrpc_install:
	make -C $(XMLRPCPATH) install
	 
$(TCLRPC): $(TCLRPCOBJS) $(XMLRPCOBJS)
	$(LD) $(LDFLAGS) -o $(TCLRPC) $(TCLRPCOBJS) $(XMLRPCPATH)/$(XMLRPCLIB)

install: $(TCLRPC)
	arm-linux-strip $(TCLRPC)
	$(ROMFSINST) /lib/$(TCLRPC)


clean:
	rm -rf *.o
	rm -rf $(TCLRPC)
