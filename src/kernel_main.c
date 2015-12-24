void SetGpioFunction(int pin, int function);
void SetGpio(int pin, int value);

void kernel_main(void)
{
  volatile int counter;
  SetGpioFunction(47, 1);
  for (;;)
  {
    SetGpio(47, 1);
    for (counter = 0x3F0000; counter > 0; --counter) {}
    SetGpio(47, 0);
    for (counter = 0x3F0000; counter > 0; --counter) {}
  }
}
