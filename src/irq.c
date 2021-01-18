#include <irq.h>
#include <rpi_peripherals.h>
#include <kernel.h>

// RPI interrupts are divided into 3 banks
#define IRQ_BANKS 3
#define IRQ_PER_BANK 32
#define MAX_ISR 10

#define NULL ((void*)0)

// Registered ISR
struct reg_isr {
  int enabled;
  int irq;
  isr_t * isr;
};

// Enable IRQ registers
reg_t * reg_enable[IRQ_BANKS] = {
  &IRQ_REGS->irq_en1,
  &IRQ_REGS->irq_en2,
  &IRQ_REGS->irq_basic_en
};

// Pending IRQ registers
reg_t * reg_pending[IRQ_BANKS] = {
  &IRQ_REGS->irq_pend1,
  &IRQ_REGS->irq_pend2,
  &IRQ_REGS->irq_basic_pend
};

// Static number of registered ISRs for now
static struct reg_isr registered_isr[MAX_ISR];

void register_isr(int irq, isr_t * handler)
{
  // Find free slot
  int idx = -1;
  for (int i = 0; i < MAX_ISR; i++)
  {
    if (!registered_isr[i].enabled)
    {
      idx = i;
      break;
    }
  }
  if (idx < 0)
  {
    kernel_panic();
  }

  // Register handler and enable interrupt
  registered_isr[idx].enabled = 1;
  registered_isr[idx].irq = irq;
  registered_isr[idx].isr = handler;
  enable_irq(irq);
}

void unregister_isr(int irq)
{
  // Find registered slot
  int idx = -1;
  for (int i = 0; i < MAX_ISR; i++)
  {
    if (registered_isr[i].enabled &&
        irq == registered_isr[i].irq)
    {
      idx = i;
      break;
    }
  }
  if (idx < 0)
  {
    kernel_panic();
  }

  // Clear handler and disable interrupt
  registered_isr[idx].enabled = 0;
  registered_isr[idx].irq = 0;
  registered_isr[idx].isr = NULL;
  disable_irq(irq);
}

// Enable/disable an IRQ
void enable_irq(int irq)
{
  int bank = irq / IRQ_PER_BANK;
  int idx = irq % IRQ_PER_BANK;

  *reg_enable[bank] |= (1 << idx);
}

void disable_irq(int irq)
{
  int bank = irq / IRQ_PER_BANK;
  int idx = irq % IRQ_PER_BANK;

  *reg_enable[bank] &= ~(1 << idx);
}


////////////////////////
// Exception handlers /
//////////////////////

void __attribute__((interrupt("UNDEF"))) illegal_instruction_isr(void)
{
  // Not implemented
  kernel_panic();
}

void __attribute__((interrupt("SWI"))) software_interrupt_isr(void)
{
  // Not implemented
  kernel_panic();
}

void __attribute__((interrupt("ABORT"))) prefetch_abort_isr(void)
{
  // Not implemented
  kernel_panic();
}

void __attribute__((interrupt("IRQ"))) interrupt_isr(void)
{
  // Check interrupt source
  for (int i = 0; i < MAX_ISR; i++)
  {
    if (registered_isr[i].enabled)
    {
      int bank = registered_isr[i].irq / IRQ_PER_BANK;
      int idx = registered_isr[i].irq % IRQ_PER_BANK;
      if (*reg_pending[bank] & (1 << idx))
      {
        // Call handler
        registered_isr[i].isr(registered_isr[i].irq);
      }
    }
  }
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_isr(void)
{
  // Not implemented
  kernel_panic();
}
