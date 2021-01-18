#include <rpi_peripherals.h>
#include <system_timer.h>

struct timer {
  unsigned int period;
  timer_callback_t * callback;
};

static struct timer timers[SYS_TIMER_COUNT];

void init_system_timer(int timer, int period, timer_callback_t * callback)
{
  // Start timer
  timers[timer].period = period;
  timers[timer].callback = callback;
  SYS_TIMER_REGS->compare[timer] = SYS_TIMER_REGS->cntLo + period;

  // Only timers 1 and 3 are supported and enabling other interrupts may be dangerous
  if (SYS_TIMER_1 == timer)
  {
    IRQ_REGS->irq_en1 |= IRQ_SYS_TIMER_1;
  }
  else if (SYS_TIMER_3)
  {
    IRQ_REGS->irq_en1 |= IRQ_SYS_TIMER_3;
  }
}

void system_timer_isr(void)
{
  if (SYS_TIMER_REGS->ctrlStatus & SYS_TIMER_M3)
  {
    // Clear interrupt
    SYS_TIMER_REGS->ctrlStatus = SYS_TIMER_M3;
    timers[SYS_TIMER_3].callback(SYS_TIMER_3);
    // Reset timer
    SYS_TIMER_REGS->compare[SYS_TIMER_3] += timers[SYS_TIMER_3].period;
  }
  else if (SYS_TIMER_REGS->ctrlStatus & SYS_TIMER_M1)
  {
    // Clear interrupt
    SYS_TIMER_REGS->ctrlStatus = SYS_TIMER_M1;
    timers[SYS_TIMER_1].callback(SYS_TIMER_1);
    // Reset timer
    SYS_TIMER_REGS->compare[SYS_TIMER_1] += timers[SYS_TIMER_1].period;
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
