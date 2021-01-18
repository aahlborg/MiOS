// Functions for reading the system timer

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

// Timers 1 and 3 are available for use by the ARM core
#define SYS_TIMER_COUNT 4
#define SYS_TIMER_1 1
#define SYS_TIMER_3 3

// Timer callback function type
typedef void (timer_callback_t)(int timer);

// Initialize system timer interrupt
void init_system_timer(int timer, int period, timer_callback_t * callback);

// Read the system timer as a 32-bit microsecond counter
unsigned int get_system_time(void);

// Read the system timer as a 64-bit microsecond counter
unsigned long long get_system_time_long(void);

// Busy wait a number of microseconds
void wait_us(unsigned int delay);

#endif
