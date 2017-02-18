// Functions for setting up and
// operating mini UART

#ifndef UART_H
#define UART_H

// Initialize the mini UART peripheral
void uart_init(int baud, int bits);

// Write a character synchronously to UART
void uart_write(char c);

#endif
