#
#	Makefile -- Build instructions for ATMEL/AT91
#

ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

ROMFS_DIRS = /firmware

SUBDIRS = hs485

export ROMFSDIR 


all:

install: mkdirs subdirs_install
	$(ROMFSINST) -p 755 /firmware/fwmap

mkdirs:
	[ -d $(ROMFSDIR) ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done

subdirs_install: $(patsubst %, subdir_%_install, $(SUBDIRS))
	
$(patsubst %, subdir_%_install, $(SUBDIRS)) :
	make -C $(patsubst subdir_%_install, %, $@) install
	
clean:

