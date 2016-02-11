# Generated automatically from Makefile.in by configure.
# Makefile.in for daemonize.
#
# $Id: Makefile.in 6623 2007-01-23 01:04:27Z bmc $
#
# This source code is released under a BSD-style license. See the LICENSE
# file for details.
# ---------------------------------------------------------------------------

srcdir		= .

ROMFSINST = $(shell if [ -x ../../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

MISSING_SOURCES	= 
MISSING_OBJECTS = $(MISSING_SOURCES:.c=.o)
CC		= arm-linux-gcc

.PHONY: all psman install clean distclean

.c.o:
	$(CC) -c $< -I.

all: daemonize 

daemonize: daemonize.o getopt.o $(MISSING_OBJECTS)
	$(CC) -o daemonize daemonize.o getopt.o $(MISSING_OBJECTS)

testdaemon: testdaemon.o
	$(CC) -o testdaemon testdaemon.o
 
psman: daemonize.ps

daemonize.ps: daemonize.1
	groff -man daemonize.1 >pstmp
	ps2ps pstmp daemonize.ps
	rm pstmp

install: all
	arm-linux-strip daemonize
	$(ROMFSINST) /sbin/daemonize

clean:
	rm -rf *.o daemonize testdaemon *.ps

distclean:	clean
	rm -f config.status config.log config.cache config.h Makefile autom?te.cache

