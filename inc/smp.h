// Multicore handling

#ifndef SMP_H
#define SMP_H

#ifdef RPI2
#define NUM_CORES 4
#define CORE_MASK 0x3
#endif

// Address of where to write the core entry point
#define CORE_MBOX_START 0x4000008C
#define CORE_MBOX_OFFSET 0x10

// Get the CPU core ID
int get_core_num(void);

// Start up a core and run func
void start_core(int core_num, void (*func)(void));

#endif
