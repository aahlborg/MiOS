#include <sys/stat.h>
#include <uart.h>

// End of kernel's static memory
extern char _end;
static char * heap_end_p = &_end;

caddr_t _sbrk(int incr)
{
  char * prev_heap_end_p;

  prev_heap_end_p = heap_end_p;
  heap_end_p += incr;

  return (caddr_t)prev_heap_end_p;
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

int _read(int file, char *ptr, int len)
{
  return 0;
}

int _write(int file, char *ptr, int len)
{
  int i;
  for (i = 0; i < len; ++i)
  {
    uart_write(*(ptr++));
  }
  return len;
}

int _lseek(int file, int ptr, int dir)
{
  return 0;
}
