// Interrupt handler system

#ifndef IRQ_H
#define IRQ_H

// Enable/disable interrupts
#define disable() asm("cpsid i")
#define enable() asm("cpsie i")

// ISR handler type
typedef void (isr_t)(int irq);

// Register/unregister interrupt handler
void register_isr(int irq, isr_t * handler);
void unregister_isr(int irq);

// Enable/disable an IRQ
void enable_irq(int irq);
void disable_irq(int irq);

#endif
