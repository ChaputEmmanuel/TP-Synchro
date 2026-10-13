.SUFFIXES: .nasm .bin .obj .c

ROOTDIR := $(shell cd .. ; pwd)

include ../make.commons
include ../make.conf # Idéalement il faudait pouvoir s'en passer
include ../make.rules

CFLAGS = $(UCFLAGS)

ASM_EXT  = nasm

ASM         = nasm
ASM_BIN_OPT = -f bin

OBJETS      = stdio.o  \
              unistd.o \
              $(MANUX_USR_INIT)

libusr.a : $(OBJETS) 
	ar rf libusr.a $(OBJETS)

clean :
	rm -f bochs.out *.bin *.obj *.o *.a

