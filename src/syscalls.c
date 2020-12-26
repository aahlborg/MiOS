#include <sys/stat.h>
#include <uart.h>
#include <kernel.h>
#include <errno.h>

// End of kernel's static memory
extern char _end;
static char * heap_end_p = &_end;

caddr_t _sbrk(int incr)
{
  char * prev_heap_end_p;

  // Extend heap
  prev_heap_end_p = heap_end_p;
  heap_end_p += incr;

  return (caddr_t)prev_heap_end_p;
}

int _close(int file)
{
  // Not implemented
  return -1;
}

int _fstat(int file, struct stat *st)
{
  // Report caracter device
  st->st_mode = S_IFCHR;
  return 0;
}

int _isatty(int file)
{
  // Yes, the only supported file is a TTY
  return 1;
}

int _read(int file, char *ptr, int len)
{
  // Not implemented
  return 0;
}

int _write(int file, char *ptr, int len)
{
  int i;
  for (i = 0; i < len; ++i)
  {
    // Write string to UART
    uart_write(*(ptr++));
  }
  return len;
}

int _lseek(int file, int ptr, int dir)
{
  // Not implemented
  return 0;
}

void _exit(int status)
{
  // Not implemented, no return
  kernel_panic();
}

int _kill(int pid, int sig)
{
  // Not implemented
  errno = EINVAL;
  return -1;
}

int _getpid(void)
{
  // Not implemented
  return 1;
}
