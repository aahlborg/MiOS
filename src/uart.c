#include <uart.h>
#include <gpio.h>
#include <rpi_peripherals.h>

void uart_init(int baud, int bits)
{
  struct rpi_aux_regs * const regs = (struct rpi_aux_regs *)AUX_BASE;

  regs->enables = AUX_EN_MU;
  regs->muIrqEnable = 0;
  regs->muExtraCtrl = 0;

  if (bits == 8)
    regs->muLineCtrl = AUX_MU_8BIT_MODE;
  else
    regs->muLineCtrl = AUX_MU_7BIT_MODE;

  regs->muModemCtrl = 0;
  regs->muIrqEnable = 0;
  regs->muIrqId = AUX_MU_IIR_CLEAR_RX | AUX_MU_IIR_CLEAR_TX;
  regs->muBaudRate = SYS_FREQ / (8 * baud) - 1;

  gpio_pin_set_function(GPIO_PIN_UART_TX, GPIO_FUNC_ALT5);
  gpio_pin_set_function(GPIO_PIN_UART_RX, GPIO_FUNC_ALT5);
  gpio_pin_set_pull_up_down(GPIO_PIN_UART_TX, GPIO_PULL_DISABLE);

  regs->muExtraCtrl = AUX_MU_CNTL_TX_ENABLE;
}

void uart_write(char c)
{
  struct rpi_aux_regs * const regs = (struct rpi_aux_regs *)AUX_BASE;

  while (0 == (regs->muLineStatus & AUX_MU_LSR_TX_EMPTY)) {}

  regs->muIO = c;
}
