#include <inc/gpio.h>
#include <inc/rpi_peripherals.h>

void gpio_pin_set_function(int pin, int function)
{
  if (pin > GPIO_PIN_MAX || function > GPIO_FUNC_MAX)
    return;

  struct rpi_gpio_regs * gpioRegs = (struct rpi_gpio_regs *)GPIO_BASE;
  const int funcReg = pin / 10;
  const int funcRegShift = (pin % 10) * 3;
  const int funcVal = function << funcRegShift;
  const int funcValMaskInv = ~(0x3 << funcRegShift);
  gpioRegs->gpioFunctionReg[funcReg] = (gpioRegs->gpioFunctionReg[funcReg] & funcValMaskInv) | funcVal;
}

void gpio_pin_write(int pin, int value)
{
  if (pin > GPIO_PIN_MAX)
    return;

  struct rpi_gpio_regs * gpioRegs = (struct rpi_gpio_regs *)GPIO_BASE;
  const int pinReg = pin / 32;
  const int shift = pin % 32;
  if (value)
  	gpioRegs->gpioPinOutputSet[pinReg] = (1 << shift);
  else
  	gpioRegs->gpioPinOutputClear[pinReg] = (1 << shift);
}
