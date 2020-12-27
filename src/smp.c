#include <smp.h>
#include <kernel.h>
#include <stdio.h>

// Core entry point table
static void (* volatile core_entry_p[4])(void) = {0};

int get_core_num(void)
{
  int core_num;
  asm volatile ("mrc p15, 0, %0, c0, c0, 5" : "=r" (core_num));
  return core_num & CORE_MASK;
}

void start_core(int core_num, void (*func)(void))
{
  if (core_num < NUM_CORES)
  {
    // Pointer to entry point function pointer
    void (**mbox)(void) = (void (**)(void))(CORE_MBOX_START + CORE_MBOX_OFFSET * core_num);

    // Check that core has not already been started
    if (core_entry_p[core_num])
      kernel_panic();

    // Save entry point address for core
    core_entry_p[core_num] = func;
    printf("Starting core %d at 0x%08x\r\n", core_num, (int)core_entry_p[core_num]);
    // Boot core from _reset
    *mbox = _reset;
  }
}

void __attribute__ ((noreturn)) _core_main(unsigned int core_num)
{
  if (core_num == 0 || core_num >= NUM_CORES)
    kernel_panic();

  // Make sure that entry point is set
  while (!core_entry_p[core_num])
  {
    // Wait for interrupt
    asm("wfi");
  }

  // Call function
  core_entry_p[core_num]();

  // In case core returns
  kernel_panic();
}
