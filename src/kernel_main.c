#include <inc/gpio.h>
#include <inc/rpi_peripherals.h>
#include <inc/system_timer.h>
#include <inc/uart.h>

void kernel_main(void)
{
  gpio_pin_set_function(GPIO_PIN_BOARD_LED, GPIO_FUNC_OUT);
  uart_init(115200, 8);
  for (;;)
  {
    gpio_pin_write(GPIO_PIN_BOARD_LED, 1);
    uart_write('U');
    uart_write('P');
    uart_write('\r');
    uart_write('\n');
    wait_us(500000);
    gpio_pin_write(GPIO_PIN_BOARD_LED, 0);
    uart_write('D');
    uart_write('N');
    uart_write('\r');
    uart_write('\n');
    wait_us(500000);
  }
}
