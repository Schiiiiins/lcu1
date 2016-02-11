#
#	Makefile -- Build instructions for ATMEL/AT91
#
#	F. Werner, 14.07.2008
#

ROMFSINST = ../helper_scripts/romfs-inst.sh

SUBDIRS = 

all:

install:
	mkdir -p $(ROMFSDIR)/www
	$(ROMFSINST) /www/cgi.tcl
	$(ROMFSINST) /www/common.tcl
	$(ROMFSINST) /www/fwupdate.cgi
	$(ROMFSINST) /www/index.cgi
	$(ROMFSINST) /www/screen.cgi
	$(ROMFSINST) /www/devconfig.cgi
	$(ROMFSINST) /www/busdevices.cgi
	$(ROMFSINST) /www/calibration.cgi
	$(ROMFSINST) /www/wireplan.cgi
	$(ROMFSINST) /www/prototype.js

clean:
