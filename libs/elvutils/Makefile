# Makefile for libelvutils

ROMFSINST = $(shell if [ -x ../../helper_scripts/romfs-inst.sh ]; then echo ../../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

CROSS = arm-linux-

ELVUTILS_SO = libelvutils.so
ELVUTILS_A = libelvutils.a

USE_MONOTONIC = 1

ELVUTILSOBJS = \
	AesCipher.o \
	BlockCipher.o \
	CryptMode.o \
	CryptModeCBC.o \
	CryptModeCFB.o \
	CryptModeECB.o \
	ConsoleLogger.o \
	FileLogger.o \
	Hexfile.o \
	Logger.o \
	OSCompatLinux.o \
	md5.o \
	MulticastSender.o \
	PropertyMap.o \
	SyslogLogger.o \
	TimerQueue.o \
	TimerTarget.o \
	TimeZoneInfo.o \
	unsigned_char_traits.o \
	utils.o \
	zeitmessung.o

ifeq "$(USE_MONOTONIC)" "1"
ELVUTILSOBJS += \
	TimerQueueMonotonic.o \
	TimerTargetMonotonic.o \
	
endif
	
ifeq "$(DEBUG)" "1"
OPTIMIZE = -g -O0
else
OPTIMIZE = -O2
endif

CC = $(CROSS)g++
CXX = $(CROSS)g++
AR = $(CROSS)ar
LD = $(CROSS)g++
STRIP = $(CROSS)strip
ARFLAGS = rcs

CXXFLAGS +=  $(OPTIMIZE) -Wall -fPIC

all: $(ELVUTILS_SO) $(ELVUTILS_A)

$(ELVUTILS_SO): $(ELVUTILSOBJS)
	$(LD) $(LDFLAGS) -shared -fPIC -o $(ELVUTILS_SO) $(ELVUTILSOBJS)


$(ELVUTILS_A): $(ELVUTILSOBJS)
	$(AR) $(ARFLAGS) $(ELVUTILS_A) $(ELVUTILSOBJS)


install: $(ELVUTILS_SO)
	$(STRIP) $(ELVUTILS_SO)
	$(ROMFSINST) /lib/$(ELVUTILS_SO)


clean:
	rm -rf *.o
	rm -rf $(ELVUTILS_SO)
	rm -rf $(ELVUTILS_A)
