// Functions for setting up and
// operating mini UART

#ifndef UART_H
#define UART_H

// UART statistics data
struct uart_stats {
  int tx_count;
  int rx_count;
  int isr_count;
};

// Initialize the mini UART peripheral
void uart_init(int baud, int bits);

// Write a character to UART Tx buffer
void uart_write_ch(char c);

// Write a character to UART Tx buffer in ISR context
void uart_write_ch_isr(char c);

// Write a string to UART Tx buffer
void uart_write(char * str, int length);

// Read from UART Rx buffer
int uart_read(char * buffer);

// Read UART statistics
struct uart_stats uart_get_stats(void);

#endif
