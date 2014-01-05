.PHONY : all install clean
all: nf2401.bin

nf2401.bin : nf2401.ihx
	objcopy -I ihex -O binary nf2401.ihx nf2401.bin

nf2401.ihx : nf2401.c
	sdcc nf2401.c

install:
	gSTCISP	

clean:
	rm  nf2401.asm nf2401.lk   nf2401.map  nf2401.omf  nf2401.rst nf2401.bin  nf2401.cdb  nf2401.ihx  nf2401.lst  nf2401.mem  nf2401.rel  nf2401.sym

