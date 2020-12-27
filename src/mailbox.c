#include <rpi_peripherals.h>
#include <trace.h>

void mailbox_write(unsigned int msg, int channel)
{
  if ((msg & ~MBOX_PTR_MASK) || channel > MBOX_CHAN_MAX)
    return;

  // Wait for mailbox to become available
  while (MBOX_REGS->status & MBOX_FULL) {}

  // Put message in mailbox
  MBOX_REGS->write = (msg | channel);
}

unsigned int mailbox_read(int channel)
{
  int msg;

  if (channel > MBOX_CHAN_MAX)
    return 0;

  for (;;)
  {
    // Wait for mailbox to become populated
    while (MBOX_REGS->status & MBOX_EMPTY) {}

    // Read message and verify channel
    msg = MBOX_REGS->read;
    if ((msg & ~MBOX_PTR_MASK) == channel)
      return (msg & MBOX_PTR_MASK);
  }
}
