#include <stdio.h>

extern void serial_putchar(char c);
extern int serial_getchar(void);

int tty_read_in(char *ptr, int len)
{
    int read = 0;
    int c;

    for (; len > 1; --len) {
        c = serial_getchar();
        if (c == '\r') {
            c = '\n';
            len = 0;
        }

        /* echo back */
        putchar(c);
        fflush(stdout);
        *ptr++ = (char)c;
        read++;
    }

    return read;
}

int tty_write_out(char *ptr, int len)
{
    int written = 0;

    for (; len != 0; --len) {
        serial_putchar(*ptr++);
        written++;
    }

    return written;
}
