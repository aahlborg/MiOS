// Functions for setting up and
// operating GPIO pins

#ifndef GPIO_H
#define GPIO_H

void gpio_pin_set_function(int pin, int function);
void gpio_pin_write(int pin, int value);
void gpio_pin_set_pull_up_down(int pin, int value);

#endif
