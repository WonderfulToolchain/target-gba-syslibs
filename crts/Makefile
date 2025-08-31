export WONDERFUL_TOOLCHAIN ?= /opt/wonderful
WF_TARGET ?= gba/rom
include ../target/gba/makedefs-common.mk

OBJDIR := build
SRCDIRS := src
MKDIRS := $(OBJDIR)

OBJECTS := \
	$(OBJDIR)/crt0_rom.o \
	$(OBJDIR)/crt0_multiboot.o
CFLAGS := -mcpu=arm7tdmi -mtune=arm7tdmi -ffreestanding
ASFLAGS := -x assembler-with-cpp
LDFLAGS :=

DESTDIR ?= $(WF)

vpath %.c $(SRCDIRS)
vpath %.S $(SRCDIRS)

.PHONY: all clean install

all: $(OBJECTS)

$(OBJDIR)/crt0_multiboot.o: crt0_agbpack.S $(OBJDIR)
	$(CC) $(ASFLAGS) -DMULTIBOOT -c -o $@ $<

$(OBJDIR)/crt0_rom.o: crt0.S $(OBJDIR)
	$(CC) $(ASFLAGS) -c -o $@ $<

$(OBJDIR):
	$(info $(shell mkdir -p $(MKDIRS)))

clean:
	rm $(OBJECTS)

install: $(OBJECTS)
	install -d $(DESTDIR)/target/gba/multiboot/lib
	install -m 644 $(OBJDIR)/crt0_multiboot.o $(DESTDIR)/target/gba/multiboot/lib/crt0.o

	install -d $(DESTDIR)/target/gba/rom/lib
	install -m 644 $(OBJDIR)/crt0_rom.o $(DESTDIR)/target/gba/rom/lib/crt0.o
