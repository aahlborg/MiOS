// Core kernel functions

#ifndef KERNEL_H
#define KERNEL_H

// The OS entry point
void _reset(void);

// OS start after setting up stack pointers, etc.
void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);

// Emergency stop
void __attribute__ ((noreturn)) kernel_panic(void);

#endif
