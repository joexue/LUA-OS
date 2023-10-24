# Makefile for Lua OS

# Copyright 2022-2023, Joe Xue(lgxue@hotmail.com)

PROJ           = luaos
SRCDIR         = src lua-5.4.4
BUILDDIR       = output
SCRIPTDIR      = script
ROOTFSDIR      = rootfs

SRC_NOT_BUILD  = lua-5.4.4/src/luac.c

SRC            = $(filter-out $(SRC_NOT_BUILD), $(shell find $(SRCDIR) -name *.c -o -name *.S -o -name *.lua))
OBJS           = $(addsuffix .o,$(addprefix $(BUILDDIR)/,$(SRC)))

FILES          = $(shell find $(ROOTFSDIR) -type f)

CFLAGS         = -g -Wall -MMD -Ilua-5.4.4/src

SYSROOT        = /opt/gcc-arm-10.3-2021.07-x86_64-aarch64-none-elf/aarch64-none-elf
CROSS_COMPILE  = $(SYSROOT)/../bin/aarch64-none-elf-
CC             = $(CROSS_COMPILE)gcc

QUIET          = @

.PHONY: all clean run

all: $(BUILDDIR)/$(PROJ).elf

$(BUILDDIR)/$(PROJ).elf: $(OBJS)
	$(QUIET)$(CC) -static -T $(SCRIPTDIR)/$(PROJ).ld $^ -nostdlib -lc -lm -lgcc -o $@

run: $(BUILDDIR)/$(PROJ).elf
	$(QUIET)qemu-system-aarch64 \
		-M virt \
		-m 16 \
		-cpu cortex-a53 \
		-nographic \
		-serial mon:stdio \
		-kernel $(BUILDDIR)/$(PROJ).elf

clean:
	-rm -rf $(BUILDDIR)

$(BUILDDIR)/%.c.o : %.c
	$(QUIET)echo $@
	$(QUIET)-mkdir -p $(@D)
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.S.o : %.S
	$(QUIET)echo $@
	$(QUIET)-mkdir -p $(@D)
	$(QUIET)$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/$(ROOTFSDIR)/ramfs: $(FILES)
	$(QUIET)echo $@
	$(QUIET)-mkdir -p $(@D)
	$(QUIET)echo "local ramfs = {}" > $@
	$(QUIET)for f in $(FILES); do \
		n=`basename $$f`; \
	   	echo "local fc = [==[" >> $@; \
		cat $$f >> $@; \
		echo "]==]" >> $@; \
		echo "table.insert(ramfs, {\"$$n\", fc})" >> $@; \
		done

# convert the lua file into c char[]
.NOTPARALLEL: $(BUILDDIR)/%.lua.o
$(BUILDDIR)/%.lua.o : %.lua $(FILES) $(BUILDDIR)/$(ROOTFSDIR)/ramfs
	$(QUIET)echo $@
	$(QUIET)-mkdir -p $(@D)
	$(QUIET)echo "unsigned char $(basename $(<F))[] = {" > $@.c
	$(QUIET)cat $(BUILDDIR)/$(ROOTFSDIR)/ramfs | xxd -i >> $@.c
	$(QUIET)echo "," >> $@.c
	$(QUIET)cat $< | xxd -i >> $@.c
	$(QUIET)echo ", 0x00 };" >> $@.c
	$(QUIET)$(CC) $(CFLAGS) -c $@.c -o $@

-include $(wildcard $(BUILDDIR)/*.d)
