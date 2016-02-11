
ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = fbshot

TARGETOBJS = $(TARGET).o

CC = arm-linux-g++
CXX = arm-linux-g++
CXXFLAGS +=  -O2 -Wall -Wstrict-prototypes 
LDFLAGS += -lpng -lz 

all: $(TARGET)

$(TARGET): $(TARGETOBJS)

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)

clean:
	rm -rf *.o
	rm -rf $(TARGET)
