#include <uart.h>
#include <system_timer.h>
#include <kernel.h>
#include <rpi_peripherals.h>

void __attribute__((interrupt("UNDEF"))) illegal_instruction_isr(void)
{
  // Not implemented
  uart_write_ch('O');
  kernel_panic();
}

void __attribute__((interrupt("SWI"))) software_interrupt_isr(void)
{
  // Not implemented
  uart_write_ch('S');
  kernel_panic();
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_isr(void)
{
  // Not implemented
  uart_write_ch('P');
  kernel_panic();
}

void __attribute__((interrupt("IRQ"))) interrupt_isr(void)
{
  // Check interrupt source
  unsigned int pend1 = IRQ_REGS->irq_pend1;
  if (pend1 & IRQ_AUX)
  {
    uart_isr();
  }
  if (pend1 & IRQ_SYS_TIMERS)
  {
    system_timer_isr();
  }
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_isr(void)
{
  // Not implemented
  uart_write_ch('F');
  kernel_panic();
}
