// Core kernel functions

#ifndef KERNEL_H
#define KERNEL_H

void _reset(void);
void _kernel_main(unsigned int r0, unsigned int r1, unsigned int atags);
void __attribute__ ((noreturn)) kernel_panic(void);

#endif
