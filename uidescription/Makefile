
ROMFSINST = $(shell if [ -x ../helper_scripts/romfs-inst.sh ]; then echo ../helper_scripts/romfs-inst.sh; else echo /usr/bin/romfs-inst.sh; fi)

DESCRIPTIONTARGETIDR = $(ROMFSDIR)/usr/share/ui

install:
	mkdir -p $(DESCRIPTIONTARGETIDR)
	find . -path '*/.svn*' -prune -o -name Makefile -prune -o -name staging -prune -o -name oss_excludes -prune -o -type d -exec mkdir -p $(DESCRIPTIONTARGETIDR)/\{\} \; -o -type f -exec cp \{\} $(DESCRIPTIONTARGETIDR)/\{\} \;


clean:
