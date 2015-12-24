#include <inc/gpio.h>
#include <inc/rpi_peripherals.h>
#include <inc/system_timer.h>

void kernel_main(void)
{
  gpio_pin_set_function(BOARD_LED_GPIO_PIN, GPIO_FUNC_OUT);
  for (;;)
  {
    gpio_pin_write(BOARD_LED_GPIO_PIN, 1);
    wait_us(500000);
    gpio_pin_write(BOARD_LED_GPIO_PIN, 0);
    wait_us(500000);
  }
}
