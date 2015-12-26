#include <rpi_peripherals.h>
#include <system_timer.h>

unsigned int get_system_time(void)
{
  const struct rpi_sys_timer_regs * const regs = (struct rpi_sys_timer_regs *)SYS_TIMER_BASE;
  return regs->cntLo;
}

unsigned long long get_system_time_long(void)
{
  const struct rpi_sys_timer_regs * const regs = (struct rpi_sys_timer_regs *)SYS_TIMER_BASE;
  return ((unsigned long long)regs->cntHi << 32) & regs->cntLo;
}

void wait_us(unsigned int delay)
{
  const struct rpi_sys_timer_regs * const regs = (struct rpi_sys_timer_regs *)SYS_TIMER_BASE;
  const unsigned int startTime = regs->cntLo;
  while (regs->cntLo - startTime < delay) {}
}
