ROMFSINST = ../helper_scripts/romfs-inst.sh
CROSS = arm-linux-

STRIP = ${CROSS}strip
TCLVERSION = $(shell awk -F "[ \t]*=[ \t]*" 'NF==2 && $$1 == "VERSION" {print $$2}' unix/Makefile)
TCLLIBRARYFILES=auto.tcl history.tcl init.tcl ldAout.tcl package.tcl safe.tcl parray.tcl word.tcl tclIndex
TCLLIBRARYFILEDIR=$(ROMFSDIR)/lib/tcl$(TCLVERSION)
TCLLIB_PACKAGES=http2.1

all:
	make CROSS=${CROSS} -C unix
	$(STRIP) unix/tclsh
	$(STRIP) unix/libtcl$(TCLVERSION).so

install: all lib_install 
	$(ROMFSINST) unix/tclsh /bin/tclsh
	$(ROMFSINST) unix/libtcl$(TCLVERSION).so /lib/libtcl$(TCLVERSION).so
	
$(TCLLIBRARYFILEDIR):
	mkdir -p $@

lib_install: $(TCLLIBRARYFILEDIR) $(patsubst %, tcllibraryfile_%_install, $(TCLLIBRARYFILES)) $(patsubst %, package_%_install, $(TCLLIB_PACKAGES))

$(patsubst %, tcllibraryfile_%_install, $(TCLLIBRARYFILES)) : $(TCLLIBRARYFILEDIR)
	$(ROMFSINST) library/$(patsubst tcllibraryfile_%_install,%,$@) /lib/tcl$(TCLVERSION)/$(patsubst tcllibraryfile_%_install,%,$@)

$(patsubst %, package_%_install, $(TCLLIB_PACKAGES)) : $(TCLLIBRARYFILEDIR)
	mkdir -p $(TCLLIBRARYFILEDIR)/$(patsubst package_%_install,%,$@)
	cp library/$(patsubst package_%_install,%,$@)/*.tcl $(TCLLIBRARYFILEDIR)/$(patsubst package_%_install,%,$@)

	
clean:
	make -C unix clean
