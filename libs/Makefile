#
#	Makefile -- Build instructions for ATMEL/AT91
#

SUBDIRS=xmlrpc elvutils xmlparser libhsscomm

CROSS = arm-linux-

all: subdirs

install: subdirs_install

clean: subdirs_clean

subdirs: $(patsubst %, subdir_%, $(SUBDIRS))

subdirs_install: $(patsubst %, subdir_%_install, $(SUBDIRS))

subdirs_clean: $(patsubst %, subdir_%_clean, $(SUBDIRS))

$(patsubst %, subdir_%, $(SUBDIRS)) :
	make CROSS=$(CROSS) -C $(patsubst subdir_%, %, $@)

$(patsubst %, subdir_%_install, $(SUBDIRS)) :
	make CROSS=$(CROSS) -C $(patsubst subdir_%_install, %, $@) install

$(patsubst %, subdir_%_clean, $(SUBDIRS)) :
	make CROSS=$(CROSS) -C $(patsubst subdir_%_clean, %, $@) clean



