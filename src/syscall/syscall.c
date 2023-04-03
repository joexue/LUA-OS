#include <stddef.h>
#include <sys/stat.h>
#include <limits.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/times.h>

extern void serial_putchar(char c);
extern int serial_getchar(void);

#define STDIN_FILENO    0
#define STDOUT_FILENO   1
#define STDERR_FILENO   2

int _execve (char *name, char **argv, char **env)
{
    errno = ENOMEM;
    return -1;
}

int _fork ()
{
    errno = EAGAIN;
    return -1;
}

int _fstat(int file, struct stat *st)
{
    if (file == 1 || file == 2)
    {
        st->st_mode = S_IFCHR;
        return 0;
    }

    errno = EBADF;
    return -1;
}

int _stat(char *file, struct stat *st)
{
    return 0;
}

int _isatty(int file)
{
    if (file == 1 || file == 2) {
        return 1;
    }

    errno = EBADF;
    return -1;
}

int _lseek(int file, int offset, int whence)
{
    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        return 0;
    }

    errno = EBADF;
    return -1;
}

void _exit(int status)
{
    int x = status / INT_MAX;
    x = 4 / x;
    for (;;)
        ;
}

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

int _getpid(void)
{
    return 1;
}

int _open(FILE *fp, int flags, int mode)
{
    errno = ENOSYS;
    return -1;
}

int _close (int file)
{
    errno = EBADF;
    return -1;
}

int _link (char *old, char *new)
{
    errno = EMLINK;
    return -1;
}

int _unlink (char *name)
{
    errno = ENOENT;
    return -1;
}

void _fini (void)
{
    return;
}

int _gettimeofday (struct timeval *tp, void *tzvp)
{
  struct timezone *tzp = tzvp;
  if (tp)
    {
      /* Ask the host for the seconds since the Unix epoch.  */
      tp->tv_sec = 0;
      tp->tv_usec = 0;
    }

  /* Return fixed data for the timezone.  */
  if (tzp)
    {
      tzp->tz_minuteswest = 0;
      tzp->tz_dsttime = 0;
    }

  return 0;
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

int _times (struct tms *buf)
{
    errno = EACCES;
    return -1;
}

void *_sbrk (ptrdiff_t incr)
{
  /* Defined by the linker.  */
  extern char    end asm ("_end");
  static char*   heap_end;
  char*          prev_heap_end;
  register char* stack_ptr asm ("sp");

  if (heap_end == NULL) {
    heap_end = &end;
  }

  prev_heap_end = heap_end;

  if ((heap_end + incr > stack_ptr)) {
      errno = ENOMEM;
      return (void *) -1;
  }

  heap_end += incr;

  return (void *) prev_heap_end;
}
