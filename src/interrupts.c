#include <uart.h>

void kernel_panic(void) __attribute__ ((noreturn));

void __attribute__((interrupt("UNDEF"))) illegal_instruction_isr(void)
{
  uart_write('O');
  kernel_panic();
}

void __attribute__((interrupt("SWI"))) software_interrupt_isr(void)
{
  uart_write('S');
  kernel_panic();
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_isr(void)
{
  uart_write('P');
  kernel_panic();
}

void __attribute__((interrupt("IRQ"))) interrupt_isr(void)
{
  uart_write('Q');
  kernel_panic();
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_isr(void)
{
  uart_write('F');
  kernel_panic();
}
