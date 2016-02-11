#
#	Makefile -- Build instructions for ATMEL/AT91
#
# Targets:
# install
#   Rebuilds everything from scratch to /tmp/at91root and builds cramfs.image
#   and an installation archive
#
# cramfs
#   Builds cramfs.image and an installation archive based on /tmp/at91root
# 
# deploy_dev
#   Puts the installation archive to the development ftp server
#
# deploy_test
#   Puts the installation archive to the test systemdevelopment ftp server
#
# deploy_productive
#   Puts the installation archive to the production system ftp server
#


UBOOT_BOARDNAME := elv_lcu1
PROJECT := lcu1
LINUXVERSION := 2.6
ROMFSDIR := /tmp/$(PROJECT)root_$(USER)

PWD := $(shell pwd)

ROMFSINST := helper_scripts/romfs-inst.sh
MKCRAMFS = $(PWD)/buildroot/output/build/cramfs-1.1/mkcramfs
MKJFFS2FS = $(PWD)/buildroot/output/host/usr/sbin/mkfs.jffs2
FAKEROOT = $(PWD)/buildroot/output/host/usr/bin/fakeroot

PATH := $(PWD)/buildroot/output/staging/usr/bin:$(PWD)/bootloader/u-boot/tools:$(PATH):/usr/sbin
export PATH

LINUXPATH := $(shell pwd)/linux-$(LINUXVERSION)/linux-$(LINUXVERSION)
CROSS := arm-linux-

export CROSS
export LINUXPATH

export ROMFSDIR 
export TFTPDIR
export DESTDIR

include VERSION

FIRMWARE_IMAGE=$(PROJECT)-firmware-$(VERSION).img
SOURCE_ARCHIVE=$(PROJECT)-oss-src-$(VERSION).tar
BUILDROOT_ROOT_TAR=buildroot/output/images/rootfs.arm.tar

SUBDIRS=bootloader linux-$(LINUXVERSION) tcl \
	webinterface drivers daemonize scripts demoapp \
	libs uiengine uidescription fbshot ubootenv \
	icc icserver iclogic pfmd hs485d devicetypes firmware tclrpc
	
all: u-boot_config subdirs

fw_oss_src_install: install oss_src

install: u-boot_config remove_installdir create_dirs 
	rm -rf root_fs.tar.gz
	$(FAKEROOT) make fakeroot_install

fakeroot_install: buildroot_install subdirs_install version_install devnodes root_fs.tar.gz jffs2.image 
	cp linux-$(LINUXVERSION)/linux-$(LINUXVERSION)/arch/arm/boot/uImage .
	tar czf $(FIRMWARE_IMAGE) root_fs.tar.gz uImage update_script

nfs_install: u-boot_config create_dirs buildroot_install subdirs_install version_install

clean: subdirs_clean

remove_installdir:
	rm -rf $(ROMFSDIR)

create_dirs: 
	mkdir -p $(ROMFSDIR)/lib 
	mkdir -p $(ROMFSDIR)/sbin

version_install:
	cp VERSION $(ROMFSDIR)/etc
	echo BUILD_TIME=`date -u +%s` >> $(ROMFSDIR)/etc/VERSION

devnodes:
	mkdir -p $(ROMFSDIR)/dev/
	mknod $(ROMFSDIR)/dev/console c 5 1
	mknod $(ROMFSDIR)/dev/ttyS0 c 4 64
	mknod $(ROMFSDIR)/dev/null c 1 3

kernel_xconfig:
	make -C linux-$(LINUXVERSION) xconfig

u-boot_config: UBOOT_CONFIGURED

UBOOT_CONFIGURED:
	make -C bootloader/u-boot $(UBOOT_BOARDNAME)_config && touch $@

subdirs: $(patsubst %, subdir_%, $(SUBDIRS))

subdirs_install: $(patsubst %, subdir_%_install, $(SUBDIRS))

subdirs_clean: $(patsubst %, subdir_%_clean, $(SUBDIRS))

$(patsubst %, subdir_%, $(SUBDIRS)) : 
	make -e -C $(patsubst subdir_%, %, $@)

$(patsubst %, subdir_%_install, $(SUBDIRS)) : 
	make -e -C $(patsubst subdir_%_install, %, $@) install

$(patsubst %, subdir_%_clean, $(SUBDIRS)) : 
	make -e -C $(patsubst subdir_%_clean, %, $@) clean

buildroot_install:
	tar xf $(BUILDROOT_ROOT_TAR) -C $(ROMFSDIR) --exclude="*/dev/*" -m

root_fs.tar.gz: $(ROMFSDIR)
	cd $(ROMFSDIR);\
	tar czf $(PWD)/root_fs.tar.gz *
	
jffs2.image: $(ROMFSDIR)
	$(MKJFFS2FS) -p -d $(ROMFSDIR) -e 128 -s 2048 -l -n -q -o $@

oss_src:
	rm -rf closed_source_excludes
	find . -path "*/.svn/text-base/*.svn-base" -print | sed 's/\/\.svn\/text-base\(\/.*\)\.svn-base/\1/' > file_list
	sed -n '/\/closed_source$$/s/closed_source/*/p' file_list > closed_source_excludes
	sed -n '/\/omit_source$$/s/omit_source/*/p' file_list >> closed_source_excludes
	sed -n '/\/oss_excludes$$/p' file_list >> closed_source_excludes
	sed -n '/\/closed_source$$/p' file_list >> closed_source_excludes
	sed -n '/\/Makefile.bindist$$/p' file_list >> closed_source_excludes
	for d in `sed -n '/\/oss_excludes$$/s/\/oss_excludes//p' file_list` ; do \
	  for f in `cat $$d/oss_excludes` ; do \
	    echo "$$d/$$f" >> closed_source_excludes ; \
	  done ; \
	done
	tar cf $(SOURCE_ARCHIVE) -X closed_source_excludes -T file_list
	TEMPDIR=`mktemp -d`;\
	for p in `sed -n '/\/closed_source$$/s/closed_source//p' file_list` ; do\
	  mkdir -p $$TEMPDIR/$$p;\
	if [ -e $$p/Makefile.bindist ]; then\
	  echo "Including binary distribution Makefile for $$p";\
	  cp $$p/Makefile.bindist $$TEMPDIR/$$p/Makefile;\
	else\
	  echo "Including null Makefile for $$p";\
	  cp closed_source_makefile $$TEMPDIR/$$p/Makefile;\
	fi;\
	BINDIST_FILES="";\
	. $$p/closed_source;\
	if [ _$$BINDIST_FILES != _ ]; then\
	  echo "Including binary files $$BINDIST_FILES for $$p";\
	  (cd $$p; cp -R $$BINDIST_FILES $$TEMPDIR/$$p);\
	fi;\
	done;\
	tar rf $(SOURCE_ARCHIVE) -C $$TEMPDIR .;\
	gzip $(SOURCE_ARCHIVE);\
	rm -rf $$TEMPDIR


