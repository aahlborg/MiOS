#include <rpi_peripherals.h>
#include <system_timer.h>

unsigned int get_system_time(void)
{
  // Return lower 32 bits of counter
  return SYS_TIMER_REGS->cntLo;
}

unsigned long long get_system_time_long(void)
{
  // Return full 64-bit counter
  return ((unsigned long long)SYS_TIMER_REGS->cntHi << 32) & SYS_TIMER_REGS->cntLo;
}

void wait_us(unsigned int delay)
{
  const unsigned int startTime = SYS_TIMER_REGS->cntLo;

  // Busy wait until number of us have passed
  while (SYS_TIMER_REGS->cntLo - startTime < delay) {}
}
