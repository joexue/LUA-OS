#include <stdio.h>
#include <stdarg.h>

static volatile unsigned int * const UARTDR = (unsigned int *)0x09000000;
static volatile unsigned int * const UARTFR = (unsigned int *)0x09000018;

#define TXFE 0x20
#define RXFE 0x10

void serial_putchar(char c)
{
    while (TXFE & *UARTFR)
        ;
    *UARTDR = c;
}

int serial_getchar(void)
{
    while (RXFE & *UARTFR)
        ;

    return *UARTDR;
}
