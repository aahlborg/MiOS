// Functions for setting up and
// operating mini UART

#ifndef UART_H
#define UART_H

// Initialize the mini UART peripheral
void uart_init(int baud, int bits);

// Write a character synchronously to UART
void uart_write(char c);

// Read from UART Rx buffer
int uart_read(char * buffer);

// UART Rx interrupt handler
void uart_rx_isr(void);

#endif
