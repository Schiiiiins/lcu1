#
#	Makefile -- Build instructions for ATMEL/AT91
#


SUBDIRS=u-boot at91bootstrap

all: subdirs

install: subdirs

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

subdir_u-boot: u-boot/u-boot.bin
	#do nothing

subdir_u-boot_clean:
	make -C u-boot clean
	rm -rf u-boot/u-boot.bin

u-boot/u-boot.bin:
	make -C u-boot elv_lcu1_config
	make -C u-boot CONFIG_NOSOFTFLOAT=1 

subdir_at91bootstrap:
	make -C at91bootstrap at91sam9261ek_defconfig
	make CROSS_COMPILE=arm-linux- -C at91bootstrap

