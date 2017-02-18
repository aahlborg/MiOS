#include <uart.h>
#include <kernel.h>

void __attribute__((interrupt("UNDEF"))) illegal_instruction_isr(void)
{
  // Not implemented
  uart_write('O');
  kernel_panic();
}

void __attribute__((interrupt("SWI"))) software_interrupt_isr(void)
{
  // Not implemented
  uart_write('S');
  kernel_panic();
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_isr(void)
{
  // Not implemented
  uart_write('P');
  kernel_panic();
}

void __attribute__((interrupt("IRQ"))) interrupt_isr(void)
{
  // Not implemented
  uart_write('Q');
  kernel_panic();
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_isr(void)
{
  // Not implemented
  uart_write('F');
  kernel_panic();
}
