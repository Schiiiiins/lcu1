# Binary distribution Makefile for pfmd

ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

TARGET = pfmd

CROSS = arm-linux-

STRIP = $(CROSS)strip

all:
	#do nothing


install: $(TARGET)
	$(STRIP) $(TARGET)
	$(ROMFSINST) /bin/$(TARGET)


clean:
	#do nothing
