#--------------------------------------------------------------
# Just run 'make menuconfig', configure stuff, then run 'make'.
# You shouldn't need to mess with anything beyond this point...
#--------------------------------------------------------------

TOPDIR=./
CONFIG_CONFIG_IN=Config.in
CONFIG_DEFCONFIG=.defconfig
CONFIG=config
CONFIG_SHELL=/bin/bash
DATE:=$(shell date +%Y%m%d)

VERSION:=2.10

noconfig_targets:= menuconfig defconfig config oldconfig

# Check first if we want to configure at91bootstrap
#
ifeq ($(filter $(noconfig_targets),$(MAKECMDGOALS)),)
-include	.config
endif

ifndef HOSTAR
HOSTAR:=ar
endif
ifndef HOSTAS
HOSTAS:=as
endif
ifndef HOSTCC
HOSTCC:=gcc
endif
ifndef HOSTCPP
HOSTCPP:=cpp
endif
ifndef HOSTLD
HOSTLD:=ld
endif
ifndef HOSTLN
HOSTLN:=ln
endif
ifndef HOSTNM
HOSTNM:=nm
endif

HOSTAR:=$(shell $(CONFIG_SHELL)	 -c "which $(HOSTAR)"  || type -p $(HOSTAR)  || echo ar)
HOSTAS:=$(shell $(CONFIG_SHELL)  -c "which $(HOSTAS)"  || type -p $(HOSTAS)  || echo as)
HOSTCC:=$(shell $(CONFIG_SHELL)  -c "which $(HOSTCC)"  || type -p $(HOSTCC)  || echo gcc)
HOSTCPP:=$(shell $(CONFIG_SHELL) -c "which $(HOSTCPP)" || type -p $(HOSTCPP) || echo cpp)
HOSTLD:=$(shell $(CONFIG_SHELL)  -c "which $(HOSTLD)"  || type -p $(HOSTLD)  || echo ld)
HOSTLN:=$(shell $(CONFIG_SHELL)  -c "which $(HOSTLN)"  || type -p $(HOSTLN)  || echo ln)
HOSTNM:=$(shell $(CONFIG_SHELL)  -c "which $(HOSTNM)"  || type -p $(HOSTNM)  || echo nm)


ifndef CFLAGS_FOR_BUILD
CFLAGS_FOR_BUILD:=-g -O2
endif
ifndef CXXFLAGS_FOR_BUILD
CXXFLAGS_FOR_BUILD:=-g -O2
endif
ifndef FCFLAGS_FOR_BUILD
FCFLAGS_FOR_BUILD:=-g -O2
endif
export HOSTAR HOSTAS HOSTCC HOSTCXX HOSTFC HOSTLD

ifeq ($(HAVE_DOT_CONFIG),)

all: menuconfig

# configuration
# ---------------------------------------------------------------------------

HOSTCFLAGS=$(CFLAGS_FOR_BUILD)
export HOSTCFLAGS

$(CONFIG)/conf:
	@mkdir -p $(CONFIG)/at91bootstrap-config
	$(MAKE) CC="$(HOSTCC)" -C $(CONFIG) conf
	-@if [ ! -f .config ]; then \
		cp $(CONFIG_DEFCONFIG) .config; \
	fi

$(CONFIG)/mconf:
	@mkdir -p $(CONFIG)/at91bootstrap-config
	$(MAKE) CC="$(HOSTCC)" -C $(CONFIG) conf mconf
	-@if [ ! -f .config ]; then \
		cp $(CONFIG_DEFCONFIG) .config; \
	fi

menuconfig: $(CONFIG)/mconf
	@mkdir -p $(CONFIG)/at91bootstrap-config
	@if ! KCONFIG_AUTOCONFIG=$(CONFIG)/at91bootstrap-config/auto.conf \
		KCONFIG_AUTOHEADER=$(CONFIG)/at91bootstrap-config/autoconf.h \
		$(CONFIG)/mconf $(CONFIG_CONFIG_IN); then \
		test -f .config.cmd || rm -f .config; \
	fi

config: $(CONFIG)/conf
	@mkdir -p $(CONFIG)/at91bootstrap-config
	@KCONFIG_AUTOCONFIG=$(CONFIG)/at91bootstrap-config/auto.conf \
		KCONFIG_AUTOHEADER=$(CONFIG)/at91bootstrap-config/autoconf.h \
		$(CONFIG)/conf $(CONFIG_CONFIG_IN)

oldconfig: $(CONFIG)/conf
	@mkdir -p $(CONFIG)/at91bootstrap-config
	@KCONFIG_AUTOCONFIG=$(CONFIG)/at91bootstrap-config/auto.conf \
		KCONFIG_AUTOHEADER=$(CONFIG)/at91bootstrap-config/autoconf.h \
		$(CONFIG)/conf -o $(CONFIG_CONFIG_IN)

defconfig: $(CONFIG)/conf
	@mkdir -p $(CONFIG)/at91bootstrap-config
	@KCONFIG_AUTOCONFIG=$(CONFIG)/at91bootstrap-config/auto.conf \
		KCONFIG_AUTOHEADER=$(CONFIG)/at91bootstrap-config/autoconf.h \
		$(CONFIG)/conf -d $(CONFIG_CONFIG_IN)

else
AS=$(CROSS_COMPILE)gcc
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
NM= $(CROSS_COMPILE)nm
SIZE=$(CROSS_COMPILE)size
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump


CHIP:=$(strip $(subst ",,$(CONFIG_CHIP)))
#"))
BOARD:=$(strip $(subst ",,$(CONFIG_BOARD)))
#"))
MACH_TYPE:=$(strip $(subst ",,$(CONFIG_MACH_TYPE)))
#"))
MEMORY:=$(strip $(subst ",,$(CONFIG_MEMORY)))
#"))
TOP_OF_MEMORY:=$(strip $(subst ",,$(CONFIG_TOP_OF_MEMORY)))
#"))
LINK_ADDR:=$(strip $(subst ",,$(CONFIG_LINK_ADDR)))
#"))
PROJECT:=$(strip $(subst ",,$(CONFIG_PROJECT)))
#"))
SPI_CLK:=$(strip $(subst ",,$(CONFIG_SPI_CLK)))
#"))
SPI_MODE:=SPI_MODE_$(strip $(subst ",,$(CONFIG_SPI_MODE)))
#"))
SPI_BOOT:=$(strip $(subst ",,$(CONFIG_SPI_BOOT)))
#"))
BOARDNAME=$(strip $(subst ",,$(CONFIG_BOARDNAME)))
#"))
CRYSTAL=$(strip $(subst ",,$(CONFIG_CRYSTAL)))
#"))
JUMP_ADDR=$(strip $(subst ",,$(CONFIG_JUMP_ADDR)))
#"))
IMG_ADDRESS=$(strip $(subst ",,$(CONFIG_IMG_ADDRESS)))
#"))
IMG_SIZE=$(strip $(subst ",,$(CONFIG_IMG_SIZE)))
#"))
CARD_SUFFIX=$(strip $(subst ",,$(CONFIG_CARD_SUFFIX)))
#"))

ifeq ($(REVISION),)
REV:=
else
REV:=-$(strip $(subst ",,$(REVISION)))
endif


obj=build/$(BOARDNAME)/

BOOT_NAME=$(BOARDNAME)-$(PROJECT)$(CARD_SUFFIX)boot-$(VERSION)$(REV)

AT91BOOTSTRAP:=binaries/$(BOOT_NAME).bin

ifeq ($(DESTDIR),)
DESTDIR=install
endif

ifeq ($(IMAGE),)
IMAGE=$(BOOT_NAME).bin
endif

ifeq ($(SYMLINK),)
SYMLINK=at91bootstrap.bin
endif


COBJS-y:= main.o $(BOARD).o
SOBJS-y:= crt0_gnu.o
DIRS:=


include 	libc/libc.mk
include		driver/driver.mk

# $(SOBJS-y:.o=.S)

SRCS	:= $(COBJS-y:.o=.c)

OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y)) 
##OBJS	:= $(addprefix $(obj),$(SOBJS-y) $(COBJS-y) $(LIBC_SOBJS-y) $(LIBC_COBJS-y) $(DRIVER_SOBJS-y) $(DRIVER_COBJS-y))
#OBJS	:= $(addprefix $(obj), $(COBJS-y))

INCL=board/$(BOARD)

#AT91_CUSTOM_FLAGS:=-mcpu=arm9
#AT91_CUSTOM_FLAGS:=-mcpu=arm926ej-s

CPPFLAGS=-g -Os -Wall 	-I$(INCL) -Iinclude			\
	-DAT91BOOTSTRAP_VERSION=\"$(VERSION)\"			\
	-D$(CHIP) -D$(BOARD) -DMACH_TYPE=$(MACH_TYPE) 		\
	-DTOP_OF_MEMORY=$(TOP_OF_MEMORY)			\
	-D$(CRYSTAL)	 -DJUMP_ADDR=$(JUMP_ADDR)		\
	-DIMG_ADDRESS=$(IMG_ADDRESS)	-DIMG_SIZE=$(IMG_SIZE)	\
	$(AT91_CUSTOM_FLAGS)

ifeq ($(CONFIG_DATAFLASH),y)
CPPFLAGS += -DCONFIG_DATAFLASH
endif

ifeq ($(CONFIG_NANDFLASH),y)
CPPFLAGS += -DCONFIG_NAND_FLASH
endif

ifeq ($(CONFIG_FLASH),y)
CPPFLAGS += -DCONFIG_FLASH
endif

ifeq ($(CONFIG_SDRAM),y)
CPPFLAGS += -DCONFIG_SDRAM
endif

ifeq ($(CONFIG_SDDRC),y)
CPPFLAGS += -DCONFIG_SDDRC
endif

ifeq ($(CONFIG_DEBUG),y)
CPPFLAGS += -DCONFIG_DEBUG
endif

ifeq ($(CONFIG_HW_INIT),y)
CPPFLAGS += -DCONFIG_HW_INIT
endif


ifeq ($(MEMORY),dataflash)
CPPFLAGS += -D$(SPI_MODE) -DAT91C_SPI_CLK=$(SPI_CLK)
CPPFLAGS += -DAT91C_SPI_PCS_DATAFLASH=$(SPI_BOOT) 
endif
ifeq ($(MEMORY),dataflashcard)
CPPFLAGS += -D$(SPI_MODE) -DAT91C_SPI_CLK=$(SPI_CLK)
CPPFLAGS += -DAT91C_SPI_PCS_DATAFLASH=$(SPI_BOOT) 
endif

ASFLAGS=-g -Os -Wall -D$(BOARD) -I$(INCL) -Iinclude  		\
	-D$(CHIP) -D$(BOARD) -DMACH_TYPE=$(MACH_TYPE) 		\
	-DTOP_OF_MEMORY=$(TOP_OF_MEMORY)			\
	-D$(CRYSTAL)	 -DJUMP_ADDR=$(JUMP_ADDR)		\
	$(AT91_CUSTOM_FLAGS)

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to map file
#  -lc 	   : 	tells the linker to tie in newlib
#  -lgcc   : 	tells the linker to tie in newlib
LDFLAGS+=-nostartfiles -Map=result/$(BOOT_NAME).map --cref
#LDFLAGS+=-lc -lgcc
LDFLAGS+=-T elf32-littlearm.lds -Ttext $(LINK_ADDR)

all: 	$(obj) $(AT91BOOTSTRAP)

$(AT91BOOTSTRAP).fixboot:	$(AT91BOOTSTRAP)
	./fixboot.py $(AT91BOOTSTRAP)

boot:	$(AT91BOOTSTRAP).fixboot

install:
	-install -d $(DESTDIR)
	install $(AT91BOOTSTRAP).fixboot $(DESTDIR)/$(IMAGE)
	-rm -f $(DESTDIR)/$(SYMLINK)
	(cd ${DESTDIR} ; \
	 ln -sf ${IMAGE} ${SYMLINK} \
	)

$(obj):
	mkdir -p $(obj)

test:
	@echo obj=$(obj)
	@echo OBJS=$(OBJS)
	@echo SRCS=$(SRCS)
	@echo "$(obj)crt0_gnu.o"

rebuild: clean all

.SUFFIXES:	.c

$(AT91BOOTSTRAP): $(obj)$(BOOT_NAME).elf
	mkdir -p binaries
	$(OBJCOPY) --strip-debug --strip-unneeded $(obj)$(BOOT_NAME).elf -O binary $(AT91BOOTSTRAP)

$(obj)$(BOOT_NAME).elf: $(OBJS)
	mkdir -p result
	$(LD) $(LDFLAGS) -n -o $(obj)$(BOOT_NAME).elf $(OBJS)

$(obj)crt0_gnu.o:  crt0_gnu.S
	$(AS)  $(ASFLAGS)  -c -o $(obj)crt0_gnu.o crt0_gnu.S 

$(obj)main.o: main.c
	$(CC) $(CPPFLAGS) -c -o $(obj)main.o main.c

$(obj)$(BOARD).o: $(obj) board/$(BOARD)/$(BOARD).c
	$(CC) $(CPPFLAGS) -c -o $(obj)$(BOARD).o board/$(BOARD)/$(BOARD).c


$(obj)string.o:	libc/string.c
	$(CC)	$(CPPFLAGS) -c -o $*.o	libc/string.c

$(obj)div0.o:	libc/div0.c
	$(CC)	$(CPPFLAGS) -c -o $*.o	libc/div0.c

$(obj)udiv.o:	libc/udiv.c
	$(CC)	$(CPPFLAGS) -c -o $*.o	libc/udiv.c

$(obj)_udivsi3.o: libc/_udivsi3.S
	$(AS)	$(ASFLAGS)  -c -o $*.o	libc/_udivsi3.S

$(obj)_umodsi3.o: $(obj) libc/_umodsi3.S
	$(AS)	$(ASFLAGS)  -c -o $*.o	libc/_umodsi3.S



$(obj)debug.o:	$(DRIVERS_SRC)/debug.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)debug.o $(DRIVERS_SRC)/debug.c

$(obj)gpio.o:	$(DRIVERS_SRC)/gpio.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)gpio.o $(DRIVERS_SRC)/gpio.c

$(obj)pmc.o:	$(DRIVERS_SRC)/pmc.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)pmc.o $(DRIVERS_SRC)/pmc.c

$(obj)sdramc.o:	$(DRIVERS_SRC)/sdramc.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)sdramc.o $(DRIVERS_SRC)/sdramc.c

$(obj)sddrc.o:	$(DRIVERS_SRC)/sddrc.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)sddrc.o $(DRIVERS_SRC)/sddrc.c

$(obj)dataflash.o: $(DRIVERS_SRC)/dataflash.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)dataflash.o $(DRIVERS_SRC)/dataflash.c

$(obj)flash.o:	$(DRIVERS_SRC)/flash.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)flash.o $(DRIVERS_SRC)/flash.c

$(obj)nandflash.o: $(DRIVERS_SRC)/nandflash.c
	$(CC)	$(CPPFLAGS) -c -o $(obj)nandflash.o $(DRIVERS_SRC)/nandflash.c


../buildroot:
	(cd .. ; svn co svn://uclibc.org/trunk/buildroot)

../buildroot/.config:	../buildroot
	cp buildroot.config $@
	make -C ../buildroot menuconfig

toolchain:	../buildroot/.config
	make -C ../buildroot
	
#	cd .. ; tar -cvf `basename $$(TOPDIR)`.tar `basename $$(TOPDIR)` ; bzip2 `basename $$(TOPDIR)`.tar )
endif

%_defconfig: $(CONFIG)/conf
	echo $(shell find ./board/ -name $@)
	cp $(shell find ./board/ -name $@) .config
#	-@$(MAKE) oldconfig

update:
	cp .config board/$(BOARD)/$(BOARDNAME)_defconfig

distrib: config-clean
	find . -type f \( -name .depend \
		-o -name '*.srec' \
		-o -name '*.elf' \
		-o -name '*.map' \
		-o -name '*.o' \
		-o -name '*~' \) \
		-print0 \
		| xargs -0 rm -f
	rm -fr result
	rm -fr build
	rm -fr .auto.deps
	rm -fr ..make.deps.tmp
	rm -fr .config.cmd .config.old
	make -C config clean
	rm -fr config/at91bootstrap-config
	rm -fr binaries
	rm -f .installed
	rm -f .configured

config-clean:
	make -C config clean
	rm -fr config/at91bootstrap-config

clean:
	find . -type f \( -name .depend \
		-o -name '*.srec' \
		-o -name '*.elf' \
		-o -name '*.map' \
		-o -name '*.o' \
		-o -name '*~' \) \
		-print0 \
		| xargs -0 rm -f
	rm -fr $(obj)
	rm -fr .config .config.cmd .config.old
	rm -fr .auto.deps
	rm -f .installed
	rm -f .configured

tarball:	clean
	F=`basename $(CURDIR)` ; cd .. ; \
	tar --force-local -cvf $$F.tar $$F; \
	rm -f $$F.tar.bz2 ; \
	bzip2 $$F.tar ; \
	cp -f $$F.tar.bz2 /usr/local/install/downloads

