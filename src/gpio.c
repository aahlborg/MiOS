#include <gpio.h>
#include <rpi_peripherals.h>

// Wait a number of cycles
static void wait_cycles(int delay)
{
  volatile int cnt = delay;
  while (--cnt) {}
}

void gpio_pin_set_function(int pin, int function)
{
  if (pin > GPIO_PIN_MAX || function > GPIO_FUNC_MAX)
    return;

  // Find pin register and bit shift
  const int funcReg = pin / 10;
  const int funcRegShift = (pin % 10) * 3;

  // Insert function value into register
  const int funcVal = function << funcRegShift;
  const int funcValMaskInv = ~(GPIO_FUNC_MASK << funcRegShift);
  GPIO_REGS->gpioFunctionReg[funcReg] = (GPIO_REGS->gpioFunctionReg[funcReg] & funcValMaskInv) | funcVal;
}

void gpio_pin_write(int pin, int value)
{
  if (pin > GPIO_PIN_MAX)
    return;

  // Find pin register and bit shift
  const int pinReg = pin / 32;
  const int shift = pin % 32;

  // Set ON or OFF
  if (value)
  	GPIO_REGS->gpioPinOutputSet[pinReg] = (1 << shift);
  else
  	GPIO_REGS->gpioPinOutputClear[pinReg] = (1 << shift);
}

void gpio_pin_set_pull_up_down(int pin, int value)
{
  if (pin > GPIO_PIN_MAX || value > GPIO_PULL_UP)
    return;

  // Find pin register and bit shift
  const int pinReg = pin / 32;
  const int shift = pin % 32;

  // Set preferred value and wait
  GPIO_REGS->gpioPinPullUpDownEnable = value;
  wait_cycles(150);
  // Configure pin and wait
  GPIO_REGS->gpioPinPullUpDownEnableClock[pinReg] = (1 << shift);
  wait_cycles(150);
  // Reset registers
  GPIO_REGS->gpioPinPullUpDownEnable = 0;
  GPIO_REGS->gpioPinPullUpDownEnableClock[pinReg] = 0;
}

void enableJTAG(void)
{
  // nTRST: GPIO22 ALT4
  gpio_pin_set_function(22, GPIO_FUNC_ALT4);
  // TDI: GPIO4 ALT5
  gpio_pin_set_function(4,  GPIO_FUNC_ALT5);
  // TMS: GPIO27 ALT4
  gpio_pin_set_function(27, GPIO_FUNC_ALT4);
  // TCK: GPIO25 ALT4
  gpio_pin_set_function(25, GPIO_FUNC_ALT4);
  // RTCK: GPIO23 ALT4
  gpio_pin_set_function(23, GPIO_FUNC_ALT4);
  // TDO: GPIO24 ALT4
  gpio_pin_set_function(24, GPIO_FUNC_ALT4);
}
