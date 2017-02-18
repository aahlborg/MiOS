// Functions for reading the system timer

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

// Read the system timer as a 32-bit microsecond counter
unsigned int get_system_time(void);

// Read the system timer as a 64-bit microsecond counter
unsigned long long get_system_time_long(void);

// Busy wait a number of microseconds
void wait_us(unsigned int delay);

#endif
