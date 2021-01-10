#include <uart.h>
#include <kernel.h>

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
  // Only UART supported for now
  uart_isr();
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_isr(void)
{
  // Not implemented
  uart_write_ch('F');
  kernel_panic();
}
