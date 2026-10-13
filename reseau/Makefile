.SUFFIXES: .nasm .bin .obj .c

include ../make.commons
include ../make.conf
include ../make.rules

ASM_BIN_OPT = -f bin

OBJETS      = reseau.o interface-reseau.o
OBJETS     += $(if $(MANUX_BUFFER_RESEAU), buffer-reseau.o)

libreseau.a : $(MANUX_FICHIER_CONFIG) $(OBJETS)
	ar rf libreseau.a $(OBJETS)

clean :
	rm -f bochs.out *.bin *.obj *.o *.a

