#include <gpio.h>
#include <rpi_peripherals.h>
#include <system_timer.h>
#include <uart.h>
#include <stdio.h>
#include <smp.h>
#include <vc_if.h>
#include <kernel.h>
#include <drawing.h>
#include <string.h>
#include <random.h>

char * modes[] = {
  "USR",       // 0x10
  "FIQ",       // 0x11
  "IRQ",       // 0x12
  "SVC",       // 0x13
  "-",         // 0x14
  "-",         // 0x15
  "-",         // 0x16
  "ABORT",     // 0x17
  "-",         // 0x18
  "-",         // 0x19
  "HYP",       // 0x1a
  "UNDEFINED", // 0x1b
  "-",         // 0x1c
  "-",         // 0x1d
  "-",         // 0x1e
  "SYSTEM"     // 0x1f
};

void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
  struct framebuffer_info fb_info;
  unsigned int value;
  unsigned int x, y;

  // Configure LED pin and UART
  // These are the only means of debugging we have
  gpio_pin_set_function(GPIO_PIN_BOARD_LED, GPIO_FUNC_OUT);
  uart_init(115200, 8);

  // Print welcome message
  printf("\r\nMiOS kernel boot\r\n");

  // Read CPSR register
  asm("mrs %0, cpsr" : "=r" (value));
  printf("CPSR: 0x%08x\r\n", value);
  printf("  Mode: 0x%02x (%s)\r\n", value & 0x3f, modes[value & 0xf]);

  enableJTAG();
  printf("Configured JTAG pins\r\n");

  // Read chip temp
  value = vc_get_temp();
  printf("Temp: %u.%u *C\r\n", value / 1000, value % 1000);
  // Add a little thermal noise to the random generator
  rnd_seed(value);

  // Set up framebuffer
  fb_info.width = 1920;
  fb_info.height = 1080;
  fb_info.bit_depth = 16;
  if (!vc_init_framebuffer(&fb_info))
    kernel_panic();
  printf("Set up framebuffer at 0x%08x\r\n", (int)fb_info.fb_pointer);
  printf("  %dx%d %d-bit\r\n", fb_info.width, fb_info.height, fb_info.bit_depth);

  // Draw random pattern
  x = fb_info.width / 2;
  y = fb_info.height / 2;
  value = 0;
  for (;;)
  {
    int dx, dy;
    unsigned int x1, y1;
    while (!(dx = random() & 0x3)) {}
    while (!(dy = random() & 0x3)) {}
    x1 = x - 2 + dx;
    y1 = y - 2 + dy;
    if (x1 >= fb_info.width || y1 >= fb_info.height)
      continue;

    draw_line(&fb_info,
      x, y, x1, y1,
      value);
    x = x1;
    y = y1;
    value++;
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
