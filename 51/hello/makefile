.PHONY : all install clean
all: hello.bin

hello.bin : hello.ihx
	objcopy -I ihex -O binary hello.ihx hello.bin

hello.ihx : hello.c
	sdcc hello.c

install:
	gSTCISP	

clean:
	rm  hello.asm hello.lk   hello.map  hello.omf  hello.rst hello.bin  hello.cdb  hello.ihx  hello.lst  hello.mem  hello.rel  hello.sym

