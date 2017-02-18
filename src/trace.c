#include <trace.h>

void _trace_hex_dump(char * buf_p, int len)
{
  int i;
  int word_size = 4;
  int line_len = 16;

  // Print address and length
  printf("0x%08x+%u:\r\n", (int)buf_p, len);

  for (i = 0; i < len; ++i)
  {
    printf("%02x", buf_p[i]);

    // Word separator
    if (word_size - 1 == i % word_size)
      printf(" ");

    // Newline
    if (line_len - 1 == i % line_len)
      printf("\r\n");
  }
  printf("\r\n");
}
