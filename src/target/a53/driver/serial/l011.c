#include <stdio.h>
#include <stdarg.h>

static volatile unsigned int * const UARTDR = (unsigned int *)0x09000000;
static volatile unsigned int * const UARTFR = (unsigned int *)0x09000018;

#define TXFE 0x20
#define RXFE 0x10

static void l011_putchar(char c)
{
    while (TXFE & *UARTFR)
        ;
    *UARTDR = c;
}

static int l011_getchar(void)
{
    while (RXFE & *UARTFR)
        ;

    return *UARTDR;
}

void serial_putchar(char c)
{
    return l011_putchar(c);
}

int serial_getchar(void)
{
    return l011_getchar();
}
