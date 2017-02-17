#include <gpio.h>
#include <rpi_peripherals.h>
#include <system_timer.h>
#include <uart.h>
#include <stdio.h>
#include <smp.h>

static volatile int core_data[NUM_CORES] = {0};

static void coreX_main(void);

void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
  gpio_pin_set_function(GPIO_PIN_BOARD_LED, GPIO_FUNC_OUT);
  uart_init(115200, 8);

  printf("MiOS kernel boot\r\n");
  printf("r0: 0x%08x\r\n", r0);
  printf("r1: 0x%08x\r\n", r1);
  printf("atags: 0x%08x\r\n", atags);

  start_core(1, coreX_main);
  start_core(2, coreX_main);
  start_core(3, coreX_main);

  for (;;)
  {
    gpio_pin_write(GPIO_PIN_BOARD_LED, 1);
    wait_us(500000);
    gpio_pin_write(GPIO_PIN_BOARD_LED, 0);
    wait_us(500000);
    printf("core_data = {0x%08x, 0x%08x, 0x%08x}\r\n", core_data[1], core_data[2], core_data[3]);
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

static void coreX_main(void)
{
  const int core_num = get_core_num();
  int i = 0;
  for (;; ++i)
  {
    core_data[core_num] = ((core_num << 28) & 0xf0000000) + (i & 0x0fffffff);
  }
}
