#include <stddef.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>

extern void serial_putchar(char c);
extern int serial_getchar(void);

static unsigned int __heap_limit = 0xcafedead;

void *_sbrk (ptrdiff_t incr)
{
  extern char   end asm ("_end"); /* Defined by the linker.  */
  static char * heap_end;
  char *        prev_heap_end;
  register char * stack_ptr asm ("sp");

  if (heap_end == NULL)
    heap_end = &end;

  prev_heap_end = heap_end;

  if ((heap_end + incr > stack_ptr)
      || (__heap_limit != 0xcafedead && heap_end + incr > (char *)__heap_limit))
  {
      errno = ENOMEM;
      return (void *) -1;
  }

  heap_end += incr;

  return (void *) prev_heap_end;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

void _exit(int status)
{
    int x = status / INT_MAX;
    x = 4 / x;
    for (;;)
        ;
}

void _kill(int pid, int sig)
{
    return;
}

int _getpid(void)
{
    return -1;
}

extern int tty_read_in (char *ptr, int len);
extern int tty_write_out (char *ptr, int len);

int _write (int file, char *ptr, int len)
{
    /* We handle the stdout and stderr only */
    if ((file != 1) && (file !=2)) {
        return -1;
    }

    return tty_write_out(ptr, len);
}

int _read(int file, char *ptr, int len)
{
    /* We handle the stdin only */
    if (file != 0) {
        return 0;
    }

    return tty_read_in(ptr, len);
}
