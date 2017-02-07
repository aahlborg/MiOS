#include <rpi_peripherals.h>
#include <trace.h>

void mailbox_write(unsigned int msg, int channel)
{
  if ((msg & ~MBOX_PTR_MASK) || channel > MBOX_CHAN_MAX)
    return;

  struct rpi_mbox_regs * const mbox0 = (struct rpi_mbox_regs *)MAILBOX_BASE;

  // Wait for mailbox to become available
  while (mbox0->status & MBOX_FULL) {}

  // Put message in mailbox
  mbox0->write = (msg | channel);
}

unsigned int mailbox_read(int channel)
{
  int msg;

  if (channel > MBOX_CHAN_MAX)
    return 0;

  struct rpi_mbox_regs * const mbox0 = (struct rpi_mbox_regs *)MAILBOX_BASE;

  for (;;)
  {
    // Wait for mailbox to become populated
    while (mbox0->status & MBOX_EMPTY) {}

    // Read message and verify channel
    msg = mbox0->read;
    if ((msg & ~MBOX_PTR_MASK) == channel)
      return (msg & MBOX_PTR_MASK);
  }
}
