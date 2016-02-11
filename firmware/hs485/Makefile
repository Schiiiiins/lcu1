
ROMFSINST = /usr/bin/romfs-inst.sh

export ROMFSDIR 

all:

install:
	find *.hex -exec $(ROMFSINST) -p 755 {} /firmware/{} \;

clean:

