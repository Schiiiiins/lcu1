# changed by eQ-3 Entwicklung GmbH 6.4.2006

# makefile written for gnu make
ROMFSINST = $(shell if [ -x ../../helper_scripts/romfs-inst.sh ]; then echo ../../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

CROSS		= arm-linux-
CXX		= $(CROSS)g++
SRC		= ./src
CPPFLAGS	= -I$(SRC)

ifeq "$(DEBUG)" "1"
OPTIMIZE = -g -O0
else
OPTIMIZE = -O2
endif
GCCWARN		= -Wall -Wstrict-prototypes
GXXWARN		= -Wall
CXXFLAGS	= $(GXXWARN) $(OPTIMIZE) $(INCLUDES) -fPIC
LD		= $(CROSS)g++
STRIP		= $(CROSS)strip
LIB		= ./libXmlRpc.a
SOLDFLAGS		= -shared -fPIC
SOLIB	= ./libXmlRpc.so


# Add your system-dependent network libs here. These are
# only used to build the tests (your application will need them too).
# Linux: none
# Solaris: -lsocket -lnsl
#SYSTEMLIBS	= -lsocket -lnsl
SYSTEMLIBS	=
LDLIBS		= $(LIB) $(SYSTEMLIBS)


OBJ		= $(SRC)/XmlRpcClient.o $(SRC)/XmlRpcDispatch.o \
		$(SRC)/XmlRpcServer.o $(SRC)/XmlRpcServerProxy.o $(SRC)/XmlRpcServerConnection.o \
		$(SRC)/XmlRpcServerMethod.o $(SRC)/XmlRpcSocket.o $(SRC)/XmlRpcSource.o \
		$(SRC)/XmlRpcUtil.o $(SRC)/XmlRpcValue.o

all:		$(LIB) $(SOLIB) 

$(LIB):		$(OBJ)
		$(AR) $(ARFLAGS) $(LIB) $(OBJ)

$(SOLIB):	$(OBJ)
		$(LD) $(SOLDFLAGS) -o $(SOLIB) $(OBJ)

install: $(SOLIB)
		$(STRIP) $(SOLIB)
		$(ROMFSINST) /lib/$(SOLIB)

tests:		$(LIB)
		cd test && $(MAKE) CXX=$(CXX) CXXFLAGS="$(CXXFLAGS)" SYSTEMLIBS="$(SYSTEMLIBS)"

doc doxygen:
		cd src && doxygen Doxyfile

clean:
		rm -f $(SRC)/*.o
		rm -f $(SRC)/*~
		rm -f $(LIB)

