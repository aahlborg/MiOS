// Functions for setting up and
// operating mini UART

#ifndef UART_H
#define UART_H

void uart_init(int baud, int bits);
void uart_write(char c);

#endif
