#include <stdio.h>

extern void serial_putchar(char c);
extern int serial_getchar(void);

int tty_read_in(char *ptr, int len)
{
    int read = 0;
    int c;

    for (; len > 0; len--) {
        c = serial_getchar();
        /* printable characters */
        if ( c >= ' ' && c <= '~') {
            putchar(c);
            fflush(stdout);
            *ptr++ = (char)c;
            read++;
            continue;
        }

        /* delete */
        if ( c == 127 ) {
            printf("\b \b");
            fflush(stdout);
            ptr = (read > 0) ? ptr - 1 : ptr;
            read = (read > 0) ? read - 1: 0;
            continue;
        }

        /* return */
        if (c == '\r') {
            c = '\n';
            putchar(c);
            fflush(stdout);
            *ptr++ = c;
            read++;
            break;
        }

        /* Ctrl-c */
        if (c == 3) {
            return EOF;
        }
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
