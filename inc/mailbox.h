// Functions for writing to and reading from mailboxes

#ifndef MAILBOX_H
#define MAILBOX_H

// Write request to mailbox
void mailbox_write(unsigned int msg, int channel);

// Read result code from mailbox
unsigned int mailbox_read(int channel);

#endif
