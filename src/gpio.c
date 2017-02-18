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
  struct rpi_gpio_regs * const regs = (struct rpi_gpio_regs *)GPIO_BASE;
  const int funcReg = pin / 10;
  const int funcRegShift = (pin % 10) * 3;

  // Insert function value into register
  const int funcVal = function << funcRegShift;
  const int funcValMaskInv = ~(GPIO_FUNC_MASK << funcRegShift);
  regs->gpioFunctionReg[funcReg] = (regs->gpioFunctionReg[funcReg] & funcValMaskInv) | funcVal;
}

void gpio_pin_write(int pin, int value)
{
  if (pin > GPIO_PIN_MAX)
    return;

  // Find pin register and bit shift
  struct rpi_gpio_regs * const regs = (struct rpi_gpio_regs *)GPIO_BASE;
  const int pinReg = pin / 32;
  const int shift = pin % 32;

  // Set ON or OFF
  if (value)
  	regs->gpioPinOutputSet[pinReg] = (1 << shift);
  else
  	regs->gpioPinOutputClear[pinReg] = (1 << shift);
}

void gpio_pin_set_pull_up_down(int pin, int value)
{
  if (pin > GPIO_PIN_MAX || value > GPIO_PULL_UP)
    return;

  // Find pin register and bit shift
  struct rpi_gpio_regs * const regs = (struct rpi_gpio_regs *)GPIO_BASE;
  const int pinReg = pin / 32;
  const int shift = pin % 32;

  // Set preferred value and wait
  regs->gpioPinPullUpDownEnable = value;
  wait_cycles(150);
  // Configure pin and wait
  regs->gpioPinPullUpDownEnableClock[pinReg] = (1 << shift);
  wait_cycles(150);
  // Reset registers
  regs->gpioPinPullUpDownEnable = 0;
  regs->gpioPinPullUpDownEnableClock[pinReg] = 0;
}
