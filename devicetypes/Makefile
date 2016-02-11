#
#	Makefile -- Build instructions for ATMEL/AT91
#

ROMFSINST = $(shell if [ -x $(CURDIR)/../helper_scripts/romfs-inst.sh ]; then echo $(CURDIR)/../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

XML_DIRS = hs485types

export ROMFSDIR 

all:
	#do nothing

install:
	for i in $(XML_DIRS); do \
		[ -d $(ROMFSDIR)/firmware/$$i ] || mkdir -p $(ROMFSDIR)/firmware/$$i; \
	done
	
	(cd bindist_files; find . -maxdepth 2 -name "*.xml" -type f -exec $(ROMFSINST) \{\} /firmware/\{\} \;)
	
	for i in $(XML_DIRS); do \
		chmod -R 755 $(ROMFSDIR)/firmware/$$i;\
	done
	
clean:

