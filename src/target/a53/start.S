.section ".text.boot"

.global _Reset
_Reset:
    ldr x30, =stack_top
    mov sp, x30

    #enable floating
    mrs x1, cpacr_el1
    mov x0, #(3 << 20)
    orr x0, x1, x0
    msr cpacr_el1, x0

    BL init
    B .
