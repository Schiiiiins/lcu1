# Makefile for AT91-Bootstrap drivers directory
DIRS				+= drivers

DRIVERS:=
DRIVERS_SRC:=driver/

COBJS-y				+=	$(DRIVERS)debug.o
COBJS-y				+=	$(DRIVERS)gpio.o
COBJS-y				+=	$(DRIVERS)pmc.o

COBJS-$(CONFIG_SDRAM)		+=	$(DRIVERS)sdramc.o
COBJS-$(CONFIG_SDDRC)		+=	$(DRIVERS)sddrc.o

COBJS-$(CONFIG_DATAFLASH)	+=	$(DRIVERS)dataflash.o
COBJS-$(CONFIG_FLASH)		+=	$(DRIVERS)flash.o
COBJS-$(CONFIG_NANDFLASH)	+=	$(DRIVERS)nandflash.o






