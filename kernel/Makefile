DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOT_DIR?=$(EXEC_PREFIX)/boot
INCLUDE_DIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -m32 -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS) -m elf_i386
LIBS:=$(LIBS) -nostdlib -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config
 
CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS)\
kernel/kernel.o
 
OBJS=\
$(KERNEL_OBJS)
 
LINK_LIST=\
$(LDFLAGS)\
$(KERNEL_OBJS)

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: SimpleOS2.kernel

SimpleOS2.kernel: $(OBJS) $(ARCHDIR)/linker.ld
	$(LD) --nmagic -T $(ARCHDIR)/linker.ld -o $@ $(LINK_LIST)
	grub-file --is-x86-multiboot2 SimpleOS2.kernel

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f SimpleOS2.kernel
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel
 
install-headers:
	mkdir -p $(DESTDIR)$(INCLUDE_DIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDE_DIR)/.

install-kernel: SimpleOS2.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp SimpleOS2.kernel $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
