# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

TOPDIR          := ../../linux-2.6/linux-2.6
DESTDIR = drivers
MAKE_FLAGS := ARCH=arm CROSS_COMPILE=arm-linux-

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g -DSCULL_DEBUG # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

#CFLAGS += $(DEBFLAGS)
#CFLAGS += -I$(LDDINC)

ifneq ($(KERNELRELEASE),)
# call from kernel build system

inout-objs := main.o

obj-m	:= inout.o

else

PWD       := $(shell pwd)

modules:
	$(MAKE) $(MAKE_FLAGS) -C $(TOPDIR) SUBDIRS=$(PWD) LDDINC=$(PWD)/../include modules

install: modules
	$(MAKE) $(MAKE_FLAGS) -C $(TOPDIR) INSTALL_MOD_PATH=$(ROMFSDIR) MOD_DESTDIR=$(DESTDIR) DEPMOD=../depmod.pl SUBDIRS=$(PWD) modules_install

endif



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend


ifeq (.depend,$(wildcard .depend))
include .depend
endif
