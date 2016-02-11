LINUXVERSION := 2.6
MAKE_FLAGS := ARCH=arm CROSS_COMPILE=arm-linux- 
PWD := $(shell pwd)


all: copy ../bootloader/u-boot/tools/mkimage
	make $(MAKE_FLAGS) -C linux-$(LINUXVERSION) oldconfig
	PATH=$(PWD)/../bootloader/u-boot/tools:$(PATH) make $(MAKE_FLAGS) -C linux-$(LINUXVERSION) modules uImage

xconfig: copy
	make $(MAKE_FLAGS) -C linux-$(LINUXVERSION) xconfig
	cp linux-$(LINUXVERSION)/.config .config 
	mv linux-$(LINUXVERSION)/.config.bak linux-$(LINUXVERSION)/.config
	
clean:
	make $(MAKE_FLAGS) -C linux-$(LINUXVERSION) clean
	rm -rf linux.bin
	rm -rf uImage

install: copy all 
	chmod 755 ./depmod.pl
	NM=arm-linux-nm make $(MAKE_FLAGS) INSTALL_MOD_PATH=$(ROMFSDIR) DEPMOD=$(PWD)/depmod.pl -C linux-$(LINUXVERSION) modules_install
	arm-linux-objcopy -O binary -S linux-$(LINUXVERSION)/vmlinux linux.bin
#	cp uImage $(ROMFSDIR)/boot
	[ _$(TFTPDIR) = _ ] || cp linux-2.6/arch/arm/boot/uImage $(TFTPDIR)/linux-$(LINUXVERSION).uImage

../bootloader/u-boot/tools/mkimage:
	make -C ../bootloader subdir_u-boot

copy:
	rm -rf linux-$(LINUXVERSION)/.config.bak
	[ -e  linux-$(LINUXVERSION)/.config ] && mv linux-$(LINUXVERSION)/.config linux-$(LINUXVERSION)/.config.bak || true
	cp .config linux-$(LINUXVERSION)/.config

