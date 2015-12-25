#include <inc/gpio.h>
#include <inc/rpi_peripherals.h>
#include <inc/system_timer.h>
#include <inc/uart.h>

static void write_str(char * str)
{
  while (*str != '\0')
  {
    uart_write(*str);
    ++str;
  }
}

void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
  gpio_pin_set_function(GPIO_PIN_BOARD_LED, GPIO_FUNC_OUT);
  uart_init(115200, 8);
  for (;;)
  {
    gpio_pin_write(GPIO_PIN_BOARD_LED, 1);
    write_str("LED on\r\n");
    wait_us(500000);
    gpio_pin_write(GPIO_PIN_BOARD_LED, 0);
    write_str("LED off\r\n");
    wait_us(500000);
  }
}
