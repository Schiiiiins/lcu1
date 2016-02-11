#
#	Makefile -- Build instructions for ATMEL/AT91
#

ROMFSINST = ../helper_scripts/romfs-inst.sh

ROMFS_DIRS = /etc/init.d /etc/templates /etc/icserver /var /tmp /mnt /root /usr/local /media/mmc /media/usb

export ROMFSDIR 

all:

install: romfs

clean:

romfs:
	[ -d $(ROMFSDIR) ] || mkdir -p $(ROMFSDIR)
	for i in $(ROMFS_DIRS); do \
		[ -d $(ROMFSDIR)/$$i ] || mkdir -p $(ROMFSDIR)/$$i; \
	done

	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/tmp
	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/etc/resolv.conf
	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/etc/init.d/S??network
	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/etc/network
	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/etc/config
	[ -z "$(ROMFSDIR)" ] || rm -rf $(ROMFSDIR)/etc/dropbear
	$(ROMFSINST) -s /var/tmp /tmp
	$(ROMFSINST) -s /var/etc/resolv.conf /etc/resolv.conf
	$(ROMFSINST) -s /usr/local/etc/config /etc/config
	$(ROMFSINST) -s /usr/local/etc/TZ /etc/TZ
	$(ROMFSINST) -s /usr/local/etc/pointercal /etc/pointercal
	$(ROMFSINST) -s /usr/local/etc/dropbear /etc/dropbear
	$(ROMFSINST) /etc/inittab
	$(ROMFSINST) /etc/fstab
	$(ROMFSINST) /etc/passwd
	$(ROMFSINST) /etc/shadow
	$(ROMFSINST) /etc/ubootenv.conf
	$(ROMFSINST) /etc/templates/TZ
	$(ROMFSINST) /etc/templates/netconfig
	$(ROMFSINST) /etc/templates/autostart
	$(ROMFSINST) -p 755 /etc/init.d/rcS
#	$(ROMFSINST) -p 755 /etc/init.d/S98sound
	$(ROMFSINST) -p 755 /etc/init.d/S91icserver
	$(ROMFSINST) -p 755 /etc/init.d/S90pfmd
	$(ROMFSINST) -p 755 /etc/init.d/S90hs485d
	$(ROMFSINST) -p 755 /etc/init.d/S91lighttpd
	$(ROMFSINST) -p 755 /etc/init.d/S92iclogic
	$(ROMFSINST) -p 755 /etc/init.d/S50dropbear
	$(ROMFSINST) -p 755 /etc/init.d/S20usbdev
	$(ROMFSINST) -p 755 /etc/init.d/S20urandom
	$(ROMFSINST) -p 755 /etc/init.d/S21inout
	$(ROMFSINST) -p 755 /etc/init.d/S10udev
	$(ROMFSINST) -p 755 /etc/init.d/S15syslog
	$(ROMFSINST) -p 755 /etc/init.d/S16klog
	$(ROMFSINST) -p 755 /etc/init.d/S11network
	$(ROMFSINST) -p 755 /etc/init.d/S11mount
	$(ROMFSINST) /etc/ts.conf
	$(ROMFSINST) /etc/templates/pointercal
	$(ROMFSINST) /etc/icserver/icserver_system.conf
	$(ROMFSINST) /etc/udhcpd.usb0.conf
	$(ROMFSINST) /etc/dnsd.conf
	$(ROMFSINST) -p 755 /bin/dhcp.script
	$(ROMFSINST) /etc/profile
	$(ROMFSINST) /etc/lighttpd.conf
	$(ROMFSINST) -p 755 /sbin/init.restart
	$(ROMFSINST) -p 755 /etc/init.d/S99application
	$(ROMFSINST) -p 755 /etc/udev/rules.d/10-udev-local.rules

