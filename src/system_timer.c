#include <rpi_peripherals.h>
#include <system_timer.h>
#include <irq.h>

struct timer {
  unsigned int period;
  timer_callback_t * callback;
};

static struct timer timers[SYS_TIMER_COUNT];

static void system_timer_isr(int irq)
{
   if (IRQ_SYS_TIMER_1 == irq)
  {
    // Clear interrupt
    SYS_TIMER_REGS->ctrlStatus = SYS_TIMER_M1;
    timers[SYS_TIMER_1].callback(SYS_TIMER_1);
    // Reset timer
    SYS_TIMER_REGS->compare[SYS_TIMER_1] += timers[SYS_TIMER_1].period;
  }
  else if (IRQ_SYS_TIMER_3 == irq)
  {
    // Clear interrupt
    SYS_TIMER_REGS->ctrlStatus = SYS_TIMER_M3;
    timers[SYS_TIMER_3].callback(SYS_TIMER_3);
    // Reset timer
    SYS_TIMER_REGS->compare[SYS_TIMER_3] += timers[SYS_TIMER_3].period;
  }
}


void init_system_timer(int timer, int period, timer_callback_t * callback)
{
  // Start timer
  timers[timer].period = period;
  timers[timer].callback = callback;
  SYS_TIMER_REGS->compare[timer] = SYS_TIMER_REGS->cntLo + period;

  // Only timers 1 and 3 are supported and enabling other interrupts may be dangerous
  if (SYS_TIMER_1 == timer)
  {
    register_isr(IRQ_SYS_TIMER_1, system_timer_isr);
  }
  else if (SYS_TIMER_3 == timer)
  {
    register_isr(IRQ_SYS_TIMER_3, system_timer_isr);
  }
}

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
