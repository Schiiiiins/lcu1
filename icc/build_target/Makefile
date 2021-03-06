# @(#)$Id: Makefile.in,v 1.52 2008/10/09 09:08:36 jw Exp $

############################################################
#
#	Copyright (C) 1985-2008  John E. Wulff
#
#   You may distribute under the terms of either the GNU General Public
#   License or the Artistic License, as specified in the README file.
#
#   For more information about this program, or for information on how
#   to contact the author, see the README file or <ic@je-wulff.de>
#
#	Makefile for immediate C compilers and runtime library
#	J.E. Wulff	14-Mar-87
#
#	use GNU make	28-Jul-96
#	use GNU autoconf and configure	6.Mar.2001
#	use GNU make pattern rules instead of suffix rules 3.Jan.2005
#
#	to make immcc, make with OPT=''
#	to make libict.a, remove all objects and make with OPT='-DLOAD -DTCP'
#	to make icr, remove all objects and make with OPT='-DRUN -DYYDEBUG'
#	to make ict, remove all objects and make with OPT='-DTCP'
#	to make lmain, remove all objects and make with OPT='-DLMAIN -DYYDEBUG'
#
#	  makeAll       # makes 'immcc' and 'libict.a' without YYDEBUG support
#	  makeAll -trm; # makes the auxiliary compilers with YYDEBUG support
#
#	Do not rm comp.c, gram.c and lexc.c with make clean in case the target
#	system has no bison or yacc and no flex or lex (some Knoppix distributions)
#	Define YYERROR_VERBOSE for bison generated files comp.c and gram.c
#	26.Mar.2008
#
############################################################

#### Start of system configuration section. ####

srcdir = ../src


CC = arm-linux-gcc
ifeq ($(CC),gcc)
O0 = -O0
else
O0 =
endif

LEX = flex
LEXLIB = 
YACC = yacc
ifeq ($(YACC),yacc)
Y = -DYACC
else
Y =
endif
INSTALL = /usr/bin/install -c -D -p
INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644

CFLAGS = -g -O2 -fsigned-char
CFLAGS_COMPILE_ONLY = -c $(OPT) $(Y) -DCC=$(CC) -fsigned-char
LDFLAGS = 
YFLAGS = -d -v
O=o

ifeq ($(findstring EFENCE,$(OPT)),EFENCE)
EL = -lefence
else
EL =
endif

LIBS = 

prefix = /usr/local
exec_prefix = ${prefix}

# Where the installed binary goes.
bindir = ${exec_prefix}/bin
binprefix =

mandir = ${prefix}/man
manext = 1
manprefix =

PSED = perl -e 'while (<>) { if ( s /^.*icc.v,v (\d+\.\d+).*$$/icc.v $$1/ ) { print; last; } }'
PREV = perl -e 'while (<>) { if ( s /^.*icc.v,v (\d+\.\d+).*$$/$$1/ ) { print; last; } }'
PMOD = perl -e 'while (<>) { if ( m /^\/tmp\/[a-z_A-Z]\w*\.o(\(\.data\+0x[a-f\d]+\))*:/ ) { s //\/tmp\/x.o:/; } print; }'
PADD = perl -e 'while (<>) { if ( m /^[a-z_A-Z]/ ) { s /^/'icc_$$rev'\//; print; } }'

#### End of system configuration section. ####

SHELL = /bin/sh

# This rule allows us to supply the necessary -D options
# in addition to whatever the user asks for.
%.$(O):	$(srcdir)/%.c
	$(CC) -I. $(CFLAGS_COMPILE_ONLY) $(CPPFLAGS) $(CFLAGS) $<

###########################################################

COBJ =	comp.$(O) genr.$(O) init.$(O) symb.$(O) outp.$(O) gram.$(O) lexc.$(O)
POBJ =	link.$(O) rsff.$(O) scan.$(O)

#### Standalone versions ###################################
#### make icr and lmain with YYDEBUG, since they need it for normal operation

im_lib: icc.h icg.pl
	@$(srcdir)/makeAll -o$(O)

quiet:
	@$(srcdir)/makeAll -q -o$(O)

immcc:	icc.$(O) misc.$(O) scid.$(O) $(COBJ) pplstfix
	$(CC) $(LDFLAGS) -o immcc icc.$(O) misc.$(O) scid.$(O) $(COBJ) $(EL)

icr:	icc.$(O) misc.$(O) scid.$(O) icr.$(O) cexe.$(O) $(COBJ) $(POBJ) icbegin.a pplstfix
	$(CC) $(LDFLAGS) -o icr icc.$(O) misc.$(O) scid.$(O) icr.$(O) cexe.$(O) $(COBJ) $(POBJ) icbegin.a $(EL)

ict:	icc.$(O) misc.$(O) scid.$(O) ict.$(O) cexe.$(O) tcpc.$(O) $(COBJ) $(POBJ) icbegin.a pplstfix
	$(CC) $(LDFLAGS) -o ict icc.$(O) misc.$(O) scid.$(O) ict.$(O) cexe.$(O) tcpc.$(O) $(COBJ) $(POBJ) icbegin.a $(EL)

#### Aux library for icr and ict ######################################

icbegin.a:	icbegin.$(O) icend.$(O)
	arm-linux-ar -rvs icbegin.a icbegin.$(O) icend.$(O)
	arm-linux-ranlib icbegin.a

#### Run-time library ######################################

libict.a:	load.$(O) misc.$(O) scid.$(O) ict.$(O) icbegin.$(O) icend.$(O) tcpc.$(O) $(POBJ)
	arm-linux-ar -rvs libict.a load.$(O) misc.$(O) scid.$(O) ict.$(O) icbegin.$(O) icend.$(O) tcpc.$(O) $(POBJ)
	arm-linux-ranlib libict.a

#### C-parser test #########################################

lmain:	lmain.$(O) misc.$(O) gram.$(O) lexc.$(O) symb.$(O)
	$(CC) $(LDFLAGS) -o lmain lmain.$(O) misc.$(O) gram.$(O) lexc.$(O) symb.$(O) $(EL)

#### Compile objects #######################################

pplstfix:
	@$(SHELL) -ec 'if [ ! -x pplstfix -a ! -L pplstfix -a -x $(srcdir)/pplstfix ]; then rm -f pplstfix; echo "ln -s $(srcdir)/pplstfix ."; ln -s $(srcdir)/pplstfix .; fi'

%.c:	%.l
    ifneq ($(LEX),)
	rm -f $@
	$(LEX) -t $< | perl $(srcdir)/gram.pl > $@
	chmod -w $@
    else
	@echo "System has no lex or flex - install one of them"
	@echo "Otherwise do not modify $< - use $@ in distribution"
	test -f $@ && touch $@
    endif

%.c:	%.y
    ifneq ($(YACC),)
	$(SHELL) -ec 'if [ -f $(*)_tab.h ]; then mv -f $(*)_tab.h y.tab.t; fi'
	rm -f y.tab.c $@
	$(YACC) $(YFLAGS) $<
	perl $*.pl y.tab.c > $@
	perl $*.pl y.tab.h > $(*)_tab.h
	$(SHELL) -ec 'if cmp -s $(*)_tab.h y.tab.t; then mv y.tab.t $(*)_tab.h; fi'
	rm -f y.tab.t y.tab.h y.tab.c
	chmod -w $(*)_tab.h $@
	mv y.output $*.output
    else
	@echo "System has no yacc, bison or byacc - install one of them"
	@echo "Otherwise do not modify $< - use $@ and $(*)_tab.h in distribution"
	test -f $@ && touch $@
    endif

%.1.gz:	%
####	@echo "$(srcdir)/pod2man -n $* -c 'iC Project Documentation' -r \"`$(PSED) $(srcdir)/scid.c`\" $< > $*.1"
	@$(srcdir)/pod2man -n $* -c 'iC Project Documentation' -r "`$(PSED) $(srcdir)/scid.c`" $< > $*.1
	@gzip -f $*.1
	
icc.h:	$(srcdir)/icc.h
	rm -rf $@
	ln -s $< $@

icg.pl:	$(srcdir)/icg.pl
	rm -rf $@
	ln -s $< $@


$(srcdir)/scid.c:	$(srcdir)/icc.v $(srcdir)/Makefile.in $(srcdir)/pawk.pl $(srcdir)/comp.pl $(srcdir)/gram.pl $(srcdir)/pplstfix $(srcdir)/../README
	rm -f $(srcdir)/scid.c; perl $(srcdir)/pawk.pl $(srcdir)/icc.v $(srcdir)/Makefile.in $(srcdir)/comp.pl $(srcdir)/gram.pl $(srcdir)/pplstfix > $(srcdir)/scid.c; chmod -w $(srcdir)/scid.c

$(srcdir)/../README:	$(srcdir)/icc.v $(srcdir)/README.tpl
	perl $(srcdir)/README.tpl $(srcdir)/icc.v > $(srcdir)/../README; 

$(srcdir)/comp.c:	$(srcdir)/comp.y $(srcdir)/comp.pl

$(srcdir)/gram.c:	$(srcdir)/gram.y $(srcdir)/gram.pl

$(srcdir)/lexc.c:	$(srcdir)/lexc.l $(srcdir)/gram.pl

$(srcdir)/cexe.c:	$(srcdir)/cexe.h
	rm -f /tmp/cexe.c
	$(SHELL) -ec 'test -f $(srcdir)/cexe.c && mv $(srcdir)/cexe.c /tmp; ./immcc -c -d20000 < /dev/null; if cmp -s cexe.c /tmp/cexe.c; then mv /tmp/cexe.c .; fi'
	$(SHELL) -ec 'if [ "$(srcdir)" != '.' ]; then mv cexe.c $(srcdir)/cexe.c; fi'
	rm -f /tmp/cexe.c

$(srcdir)/immcc.1.gz:	$(srcdir)/icc.c $(srcdir)/scid.c
####	@echo "$(srcdir)/pod2man -n immcc -c 'iC Project Documentation' -r \"`$(PSED) $(srcdir)/scid.c`\" $< > $(srcdir)/immcc.1"
	@$(srcdir)/pod2man -n immcc -c 'iC Project Documentation' -r "`$(PSED) $(srcdir)/scid.c`" $< > $(srcdir)/immcc.1
	@gzip -f $(srcdir)/immcc.1

$(srcdir)/immac.1.gz:	$(srcdir)/immac $(srcdir)/scid.c

$(srcdir)/iCbox.1.gz:	$(srcdir)/iCbox $(srcdir)/scid.c

$(srcdir)/iClift.1.gz:	$(srcdir)/iClift $(srcdir)/scid.c

$(srcdir)/iClive.1.gz:	$(srcdir)/iClive $(srcdir)/scid.c

$(srcdir)/iCman.1.gz:	$(srcdir)/iCman $(srcdir)/scid.c

$(srcdir)/iCserver.1.gz:	$(srcdir)/iCserver $(srcdir)/scid.c

$(srcdir)/iCmake.1.gz:	$(srcdir)/iCmake $(srcdir)/scid.c

$(srcdir)/makeAll.1.gz:	$(srcdir)/makeAll $(srcdir)/scid.c

test:
	@cd ${srcdir}/Test0/; rm -f `cat pg.lt`; cd - > /dev/null;
	@echo "compile and generate all executables from iC and iCa files in ${srcdir}/Test0 ..."
	@-${srcdir}/iCmake -Ajfw. ${srcdir}/Test0/*.ic* 2>&1 | $(PMOD) > ${srcdir}/Test0/Init.out 2>&1
	@echo "diffing generated files in ${srcdir}/Test0 with originals in ${srcdir}/Test0/ORG ..."
	@if ${srcdir}/Mdiff -kel -Ibytes -Isyntax.error -Iparse.error ${srcdir}/Test0/*.* ${srcdir}/Test0/ORG; then echo "test OK"; if ! ${srcdir}/iCserver -h 2> /dev/null; then echo "- suggest you install Time::HiRes as described in the README followed by"; fi; echo "- make install as su"; else echo "test FAILED - differences in listed files and ${srcdir}/Test0/ORG"; fi

strict:
	@cd ${srcdir}/Test0/; rm -f `cat pg.lt`; cd - > /dev/null;
	@echo "compile with -S option and generate all executables from iC and iCa files in ${srcdir}/Test0 ..."
	@-${srcdir}/iCmake -SAjfw. ${srcdir}/Test0/*.ic* 2>&1 | $(PMOD) > ${srcdir}/Test0/Init.out 2>&1
	@echo "diffing generated files in ${srcdir}/Test0 with originals in ${srcdir}/Test0/STRICT ..."
	@if ${srcdir}/Mdiff -kLel -Ibytes -Isyntax.error -Iparse.error ${srcdir}/Test0/*.* ${srcdir}/Test0/STRICT; then echo "test OK"; if ! ${srcdir}/iCserver -h 2> /dev/null; then echo "- suggest you install Time::HiRes as described in the README followed by"; fi; echo "- make install as su"; else echo "test FAILED - differences in listed files and ${srcdir}/Test0/STRICT"; fi

install:	$(srcdir)/immcc.1.gz $(srcdir)/immac.1.gz $(srcdir)/iCbox.1.gz $(srcdir)/iClift.1.gz $(srcdir)/iClive.1.gz $(srcdir)/iCmake.1.gz $(srcdir)/iCman.1.gz $(srcdir)/iCserver.1.gz $(srcdir)/makeAll.1.gz
	$(srcdir)/perlinstall -t $(srcdir)
	${INSTALL_PROGRAM} $(srcdir)/Msg.pm    `$(srcdir)/perlinstall`/Msg.pm
	${INSTALL_PROGRAM} $(srcdir)/immcc      ${bindir}/${binprefix}immcc
	${INSTALL_PROGRAM} $(srcdir)/pplstfix   ${bindir}/${binprefix}pplstfix
	${INSTALL_PROGRAM} $(srcdir)/immac      ${bindir}/${binprefix}immac
	${INSTALL_PROGRAM} $(srcdir)/iCbox      ${bindir}/${binprefix}iCbox
	${INSTALL_PROGRAM} $(srcdir)/iClift     ${bindir}/${binprefix}iClift
	${INSTALL_PROGRAM} $(srcdir)/iClive     ${bindir}/${binprefix}iClive
	${INSTALL_PROGRAM} $(srcdir)/iCmake     ${bindir}/${binprefix}iCmake
	${INSTALL_PROGRAM} $(srcdir)/iCman      ${bindir}/${binprefix}iCman
	${INSTALL_PROGRAM} $(srcdir)/iCserver   ${bindir}/${binprefix}iCserver
	${INSTALL_DATA} $(srcdir)/libict.a      ${prefix}/lib/libict.a
	${INSTALL_DATA} $(srcdir)/icg.h         ${prefix}/include/icg.h
	${INSTALL_DATA} $(srcdir)/immcc.1.gz    ${prefix}/man/man1/immcc.1.gz
	${INSTALL_DATA} $(srcdir)/immac.1.gz    ${prefix}/man/man1/immac.1.gz
	${INSTALL_DATA} $(srcdir)/iCbox.1.gz    ${prefix}/man/man1/iCbox.1.gz
	${INSTALL_DATA} $(srcdir)/iClift.1.gz   ${prefix}/man/man1/iClift.1.gz
	${INSTALL_DATA} $(srcdir)/iClive.1.gz   ${prefix}/man/man1/iClive.1.gz
	${INSTALL_DATA} $(srcdir)/iCmake.1.gz   ${prefix}/man/man1/iCmake.1.gz
	${INSTALL_DATA} $(srcdir)/iCman.1.gz    ${prefix}/man/man1/iCman.1.gz
	${INSTALL_DATA} $(srcdir)/iCserver.1.gz ${prefix}/man/man1/iCserver.1.gz
	${INSTALL_DATA} $(srcdir)/makeAll.1.gz  ${prefix}/man/man1/makeAll.1.gz

uninstall:
	$(srcdir)/perlinstall -r $(srcdir)
	rm -f `$(srcdir)/perlinstall`/Msg.pm
	rm -f ${bindir}/${binprefix}immcc
	rm -f ${bindir}/${binprefix}pplstfix
	rm -f ${bindir}/${binprefix}iCbox
	rm -f ${bindir}/${binprefix}iClift
	rm -f ${bindir}/${binprefix}iClive
	rm -f ${bindir}/${binprefix}iCmake
	rm -f ${bindir}/${binprefix}iCman
	rm -f ${bindir}/${binprefix}iCserver
	rm -f ${prefix}/lib/libict.a
	rm -f ${prefix}/include/icg.h
	rm -f ${prefix}/man/man1/immcc.1.gz
	rm -f ${prefix}/man/man1/immac.1.gz
	rm -f ${prefix}/man/man1/iCbox.1.gz
	rm -f ${prefix}/man/man1/iClift.1.gz
	rm -f ${prefix}/man/man1/iClive.1.gz
	rm -f ${prefix}/man/man1/iCmake.1.gz
	rm -f ${prefix}/man/man1/iCman.1.gz
	rm -f ${prefix}/man/man1/iCserver.1.gz
	rm -f ${prefix}/man/man1/makeAll.1.gz

clean:
	rm -rf *.$(O) IcrObjs IctObjs LibObjs LmainObjs cexe.c scid.c icg.h core immcc libict.a icbegin.a icr ict lmain *.1.gz pplstfix

distclean: clean
	rm -f $(srcdir)/Makefile $(srcdir)/comp.c $(srcdir)/gram.c $(srcdir)/lexc.c $(srcdir)/config.status $(srcdir)/config.log $(srcdir)/config.cache $(srcdir)/../README 

distribution:
	@rev=`$(PREV) $(srcdir)/scid.c`; echo "making icc_$$rev.tgz"; cd ${srcdir}/../..; tar -czf icc_$$rev.tgz `$(PADD) icc_$$rev/src/manifest`; cd -;

genr.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h $(srcdir)/comp_tab.h
init.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h $(srcdir)/comp_tab.h
symb.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h $(srcdir)/comp_tab.h

comp.$(O):	$(srcdir)/comp.c $(srcdir)/icc.h $(srcdir)/comp.h
	$(CC) -I. $(CFLAGS_COMPILE_ONLY) -DYYERROR_VERBOSE $(CPPFLAGS) $(CFLAGS) $(O0) $<

icc.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h $(srcdir)/cexe.h
outp.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h
cexe.$(O):	$(srcdir)/cexe.c $(srcdir)/icc.h $(srcdir)/comp.h

ict.$(O):	$(srcdir)/icc.h $(srcdir)/tcpc.h
load.$(O):	$(srcdir)/icc.h $(srcdir)/tcpc.h

icr.$(O):	$(srcdir)/icc.h
link.$(O):	$(srcdir)/icc.h
rsff.$(O):	$(srcdir)/icc.h
scan.$(O):	$(srcdir)/icc.h

gram.$(O):	$(srcdir)/gram.c $(srcdir)/icc.h $(srcdir)/comp.h
	$(CC) -I. $(CFLAGS_COMPILE_ONLY) -DYYERROR_VERBOSE $(CPPFLAGS) $(CFLAGS) $(O0) $<

lexc.$(O):	$(srcdir)/lexc.c $(srcdir)/icc.h $(srcdir)/comp.h $(srcdir)/gram_tab.h
	$(CC) -I. $(CFLAGS_COMPILE_ONLY) $(CPPFLAGS) $(CFLAGS) $(O0) $<

lmain.$(O):	$(srcdir)/icc.h $(srcdir)/comp.h

misc.$(O):	$(srcdir)/icc.h

scid.$(O):	$(srcdir)/scid.c
