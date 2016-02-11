ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = iclogic

TARGETOBJS = $(TARGET).o helper_functions.o

IMMCC = ../icc/build_host/immcc
IMMAC = ../icc/src/immac
IMMCLIBPATH = ../icc/build_target
IMMCLIB = ict
IMMCINCPATH = ../icc/build_target

CC = arm-linux-gcc
CXX = arm-linux-g++
LINK.o = arm-linux-g++
CFLAGS +=  -fno-builtin -fsigned-char -I$(IMMCINCPATH) 
CXXFLAGS +=  -O2 -I$(IMMCINCPATH)
LDLIBS += -fno-builtin -lict -L$(IMMCLIBPATH) -lm


all: $(TARGET)

$(TARGET): $(TARGETOBJS)

iClogic.c $(TARGETOBJS): \
				external_signals.ih \
				defines.ih \
				alarmclock.ic \
				monoflop.ic \
                controller.ic \
                doorlock.ic \
				Makefile

%.ih: %.iha
	$(IMMAC) -o $@ $<

%.ic: %.ica
	$(IMMAC) -o $@ $<

external_signals.iha: ../scripts/icserver_system.conf
	./xml2iha.tcl $< $@

install: $(TARGET)
	arm-linux-strip $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)


clean:
	rm -rf *.o
	rm -rf $(TARGET)
	rm -rf $(TARGET).c external_signals.ih*

%.c:	%.ic
	$(IMMCC) -o $@ $<
