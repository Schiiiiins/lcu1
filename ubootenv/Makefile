ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = ubootenv

TARGETOBJS = $(TARGET).o crc32.o

LIBELVUTILSPATH = ../libs/elvutils
LIBELVUTILS = elvutils

CC = arm-linux-g++
CXX = arm-linux-g++
CXXFLAGS +=  -O2 -Wall -I../bootloader/u-boot/include -I$(LIBELVUTILSPATH) 
LDFLAGS += -L$(LIBELVUTILSPATH) -l$(LIBELVUTILS) 

all: $(TARGET)

$(TARGET): $(TARGETOBJS)

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
