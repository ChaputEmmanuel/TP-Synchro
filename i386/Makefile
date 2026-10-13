.SUFFIXES: .nasm .bin .obj .c .elf

include ../make.conf
include ../make.commons
include ../make.rules

# Les objects compilés (éventuellement sous condition d'une variable)
OBJETS      = interruptions.o \
              horloge.o \
              intel-8259a.o \
              interBasNiveau.elf \
#              arith64.o \
#              gestionGDT.elf

OBJETS     +=   $(if $(MANUX_GESTION_MEMOIRE), segment.o)

libi386.a : $(OBJETS)
	ar rf libi386.a	$(OBJETS)

clean :
	rm -f bochs.out *.bin *.obj *.o *.a *.elf

