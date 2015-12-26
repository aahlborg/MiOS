#include <gpio.h>
#include <rpi_peripherals.h>
#include <system_timer.h>
#include <uart.h>
#include <stdio.h>

void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
  gpio_pin_set_function(GPIO_PIN_BOARD_LED, GPIO_FUNC_OUT);
  uart_init(115200, 8);

  printf("MiOS kernel boot\r\n");
  printf("r0: 0x%08x\r\n", r0);
  printf("r1: 0x%08x\r\n", r1);
  printf("atags: 0x%08x\r\n", atags);

  for (;;)
  {
    gpio_pin_write(GPIO_PIN_BOARD_LED, 1);
    printf('1');
    wait_us(500000);
    gpio_pin_write(GPIO_PIN_BOARD_LED, 0);
    printf('0');
    wait_us(500000);
  }
}

void __attribute__ ((noreturn)) kernel_panic(void)
{
  // Flash board LED at 5 Hz
  for (;;)
  {
    gpio_pin_write(GPIO_PIN_BOARD_LED, 1);
    wait_us(100000);
    gpio_pin_write(GPIO_PIN_BOARD_LED, 0);
    wait_us(100000);
  }
}
