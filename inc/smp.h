// Multicore handling

#ifndef SMP_H
#define SMP_H

#ifdef RPI2
#define NUM_CORES 4
#define CORE_MASK 0x3
#endif

#define CORE_MBOX_START 0x4000008C
#define CORE_MBOX_OFFSET 0x10

int get_core_num(void);
void start_core(int core_num, void (*func)(void));

#endif
