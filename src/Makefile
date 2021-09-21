include /usr/include/n64/make/PRdefs

OPTIMIZER  = -O2
LCDEFS     = -D_FINALROM -DNDEBUG -DF3DEX_GBI_2
N64LIB     = -lultra_rom

ifeq ($(LANG), LANG_PTBR)
LCDEFS += -DLANG_PTBR
endif

# for nustd
NUSTDINCDIR = $(N64KITDIR)/nustd/include
NUSTDLIBDIR = $(N64KITDIR)/nustd/lib

ELF		= dungeon64.elf
TARGETS	= ../dungeon64.z64
MAP		= dungeon64.map

LD_SCRIPT	 = dungeon64.ld
CP_LD_SCRIPT = dungeon64_cp.ld

ASMFILES   := $(wildcard asm/*.s)
ASMOBJECTS  = $(ASMFILES:.s=.o)

# these files will be compiled and linked
CODEFILES      := $(wildcard *.c)
OBJECTFILES    := $(wildcard objects/*.c)
COMBATOBJFILES := $(wildcard objects/combat/*.c)
MAPFILES       := $(wildcard maps/*.c)
FONTFILES      := $(wildcard fonts/*.c)
SCENEFILES     := $(wildcard scenes/*.c)
UTILFILES      := $(wildcard util/*.c)
COMBATFILES    := $(wildcard combat/*.c)
ITEMFILES      := $(wildcard items/*.c)
U64EXTFILES    := $(wildcard ../libs/ultra64-extensions/src/*.c)
CODEOBJECTS     = $(CODEFILES:.c=.o) $(OBJECTFILES:.c=.o) $(COMBATOBJFILES:.c=.o) $(MAPFILES:.c=.o) $(FONTFILES:.c=.o)
CODEOBJECTS    += $(SCENEFILES:.c=.o) $(UTILFILES:.c=.o) $(COMBATFILES:.c=.o) $(ITEMFILES:.c=.o) $(U64EXTFILES:.c=.o)

# these files will be compiled, but NOT linked
DATAFILES  := $(wildcard data/*.c)
DATAOBJECTS = $(DATAFILES:.c=.o)

BOOT	    = /usr/lib/n64/PR/bootcode/boot.6102
BOOT_OBJ    = boot.6102.o
CODESEGMENT = codesegment.o

OBJECTS = $(CODESEGMENT) $(DATAOBJECTS) $(ASMOBJECTS) $(BOOT_OBJ)

LCDEFS += $(HW_FLAGS)
LCINCS  = -I. -I$(NUSTDINCDIR) -I/usr/include/n64/PR
LCOPTS  = -fno-toplevel-reorder -fno-zero-initialized-in-bss
LDFLAGS = -L/usr/lib/n64 $(N64LIB) -L$(N64_LIBGCCDIR) -lgcc -L$(NUSTDLIBDIR) -lnustd_d

# When testing is needed. Gives false positives, so we keep it commented most of the time.
# LCOPTS += -Wall -Wno-missing-braces

LDIRT  = $(ELF) $(CP_LD_SCRIPT) $(MAP) $(ASMOBJECTS) $(TARGETS)

default: $(TARGETS)

ptbr:
	@make -f Makefile LANG=LANG_PTBR

cleaner:
	find ../ -name '*.o' -delete
	find ../ -name '*.gen.h' -delete

include $(COMMONRULES)

.s.o:
	$(AS) -Wa,-Iasm -o $@ $<

$(CODESEGMENT):	$(CODEOBJECTS)
	$(LD) -o $(CODESEGMENT) -r $(CODEOBJECTS) $(LDFLAGS)

$(BOOT_OBJ): $(BOOT)
	$(OBJCOPY) -I binary -B mips -O elf32-bigmips $< $@

$(CP_LD_SCRIPT): $(LD_SCRIPT)
	cpp -P -Wno-trigraphs -o $@ $<

$(TARGETS): $(OBJECTS) $(CP_LD_SCRIPT)
	$(LD) -L. -T $(CP_LD_SCRIPT) -Map $(MAP) -o $(ELF) 
	$(OBJCOPY) --pad-to=0x100000 --gap-fill=0xFF $(ELF) $(TARGETS) -O binary
	makemask $(TARGETS)
