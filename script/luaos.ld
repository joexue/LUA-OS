/*
 * 16M memory map
 * qemu starts a53 kernel from 0x40080000
 */

ENTRY(_Reset)
SECTIONS
{
    . = 0x40080000;
    .text.boot : { *(.text.boot) }
    .text : ALIGN(8) { *(.text) }
    .rodata : ALIGN(8) { *(.rodata) }
    .data : ALIGN(8) { *(.data) }
    .bss  : ALIGN(8) { *(.bss*)  }
    . = ALIGN(8);
    _end = .;
    . = 0x40000000 + 0x1000000;
    stack_top = .;
}
