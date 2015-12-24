// Functions for reading the system timer

#ifndef SYSTEM_TIMER_H
#define SYSTEM_TIMER_H

unsigned int get_system_time(void);
unsigned long long get_system_time_long(void);
void wait_us(unsigned int delay);

#endif
