#
#	Makefile -- Build instructions for ATMEL/AT91
#

SUBDIRS=communication inout

all: subdirs

install: subdirs_install

clean: subdirs_clean

subdirs: $(patsubst %, subdir_%, $(SUBDIRS))

subdirs_install: $(patsubst %, subdir_%_install, $(SUBDIRS))

subdirs_clean: $(patsubst %, subdir_%_clean, $(SUBDIRS))

$(patsubst %, subdir_%, $(SUBDIRS)) :
	make -C $(patsubst subdir_%, %, $@)

$(patsubst %, subdir_%_install, $(SUBDIRS)) :
	make -C $(patsubst subdir_%_install, %, $@) install

$(patsubst %, subdir_%_clean, $(SUBDIRS)) :
	make -C $(patsubst subdir_%_clean, %, $@) clean



