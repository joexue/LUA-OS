# Makefile for AOS

# Copyright 2022, Joe Xue(lgxue@hotmail.com)

PROJ           = luaos
SRCDIR         = src
BUILDDIR       = build
SCRIPTDIR      = script

SRC_NOT_BUILD  =

SRC            = $(filter-out $(SRC_NOT_BUILD), $(shell find $(SRCDIR) -name *.c -o -name *.S))
OBJS           = $(addsuffix .o,$(addprefix $(BUILDDIR)/,$(SRC)))

CFLAGS         = -g -Wall -MMD

SYSROOT        = /opt/arm-gnu-toolchain-12.2.rel1-x86_64-aarch64-none-elf/aarch64-none-elf
CROSS_COMPILE  = $(SYSROOT)/../bin/aarch64-none-elf-
CC             = $(CROSS_COMPILE)gcc

.PHONY: all clean run

all: $(BUILDDIR)/$(PROJ).elf

$(BUILDDIR)/$(PROJ).elf: $(OBJS)
	$(CC) -static -T $(SCRIPTDIR)/$(PROJ).ld $^ -nostdlib -lc -lgcc -o $@

run: $(BUILDDIR)/$(PROJ).elf
	@qemu-system-aarch64 \
		-M virt \
		-m 128 \
		-cpu cortex-a53 \
		-nographic \
		-serial mon:stdio \
		-kernel $(BUILDDIR)/$(PROJ).elf

clean:
	-rm -rf $(BUILDDIR)

$(BUILDDIR)/%.o : %
	echo $@
	-mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(wildcard $(BUILDDIR)/*.d)
