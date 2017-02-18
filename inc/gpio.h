// Functions for setting up and
// operating GPIO pins

#ifndef GPIO_H
#define GPIO_H

// Set pin function
void gpio_pin_set_function(int pin, int function);

// Write value to pin
void gpio_pin_write(int pin, int value);

// Set pull state for input pin
void gpio_pin_set_pull_up_down(int pin, int value);

#endif
