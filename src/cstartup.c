extern int __bss_start__;
extern int __bss_end__;

extern void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);

void _cstartup(unsigned int r0, unsigned int r1, unsigned int atags)
{
  // volatile or GCC will optimize using memset, disable for now
  volatile int * bss_p = &__bss_start__;
  const int * const bss_end_p = &__bss_end__;

  while (bss_p < bss_end_p)
  {
    *(bss_p++) = 0;
  }

  _kernel_main(r0, r1, atags);
}
