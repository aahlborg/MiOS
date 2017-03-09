#include <vc_if.h>
#include <stddef.h>
#include <string.h>
#include <mailbox.h>
#include <rpi_peripherals.h>
#include <trace.h>

// For codes and tag IDs, see
// https://github.com/raspberrypi/firmware/wiki/Mailboxes

#define MAX_FRAME_SIZE 4096
#define MAX_BIT_DEPTH  32

#define REQUEST_CODE   0x00000000
#define RESPONSE_OK    0x80000000
#define RESPONSE_ERROR 0x80000001

#define TAG_FIRMWARE_REV    0x00000001
#define TAG_TEMPERATURE     0x00030006
#define TAG_ALLOCATE_BUFFER 0x00040001
#define TAG_GET_PHYS_SIZE   0x00040003
#define TAG_GET_BIT_DEPTH   0x00040005
#define TAG_GET_PITCH       0x00040008
#define TAG_SET_PHYS_SIZE   0x00048003
#define TAG_SET_VIRT_SIZE   0x00048004
#define TAG_SET_BIT_DEPTH   0x00048005
#define TAG_END             0x00000000

#define TEMPERATURE_ID 0

#define ARM_ADDR_SPACE_MASK 0x3fffffff

static unsigned int buffer[64] __attribute__((aligned(16)));

// Read a single tag from the buffer
unsigned int * get_tag(unsigned int * buf, unsigned int tag)
{
  int i;
  for (i = 2; i < buf[0]; ++i)
  {
    if ((tag == buf[i]) && (buf[i + 2] & RESPONSE_OK))
    {
      return &buf[i + 3];
    }
  }
  return NULL;
}

int vc_init_framebuffer(struct framebuffer_info * fb_info_p)
{
  int result;
  int i = 0;
  unsigned int * val_p;

  if (fb_info_p->width > MAX_FRAME_SIZE ||
      fb_info_p->height > MAX_FRAME_SIZE ||
      fb_info_p->bit_depth > MAX_BIT_DEPTH)
    return 0;

  buffer[i++] = 0; // Set size later
  buffer[i++] = REQUEST_CODE;

  buffer[i++] = TAG_ALLOCATE_BUFFER;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = 128; // Alignment, set to cache line size
  buffer[i++] = 0;

  buffer[i++] = TAG_SET_PHYS_SIZE;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = fb_info_p->width;
  buffer[i++] = fb_info_p->height;

  buffer[i++] = TAG_SET_VIRT_SIZE;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = fb_info_p->width;
  buffer[i++] = fb_info_p->height;

  buffer[i++] = TAG_SET_BIT_DEPTH;
  buffer[i++] = 4;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = fb_info_p->bit_depth;

  buffer[i++] = TAG_GET_PITCH;
  buffer[i++] = 4;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = 0;

  buffer[i++] = TAG_GET_PHYS_SIZE;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = 0;
  buffer[i++] = 0;

  buffer[i++] = TAG_GET_BIT_DEPTH;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = 0;

  buffer[i++] = TAG_END;
  buffer[0] = i * sizeof(int);

  // Write request and read back result
  mailbox_write((int)&buffer[0], MBOX_CHAN_VC_IF);
  result = mailbox_read(MBOX_CHAN_VC_IF);
  if (0 == result)
    return 0;

  // Verify fb configuration parameters
  val_p = get_tag(buffer, TAG_GET_PITCH);
  if (val_p)
  {
    TRACE_DEBUG("Framebuffer pitch %u\r\n", val_p[0]);
    fb_info_p->pitch = val_p[0];
  }
  else
    return 0;

  val_p = get_tag(buffer, TAG_GET_PHYS_SIZE);
  if (val_p)
  {
    TRACE_DEBUG("Framebuffer size %ux%u\r\n", val_p[0], val_p[1]);
    fb_info_p->width = val_p[0];
    fb_info_p->height = val_p[1];
  }
  else
    return 0;

  val_p = get_tag(buffer, TAG_GET_BIT_DEPTH);
  if (val_p)
  {
    TRACE_DEBUG("Framebuffer depth %u\r\n", val_p[0]);
    fb_info_p->bit_depth = val_p[0];
  }
  else
    return 0;

  val_p = get_tag(buffer, TAG_ALLOCATE_BUFFER);
  if (val_p)
  {
    TRACE_DEBUG("Framebuffer addr 0x%08x+%u\r\n", val_p[0], val_p[1]);
    // Translate address to ARM address space
    fb_info_p->fb_pointer = (void *)(val_p[0] & ARM_ADDR_SPACE_MASK);
    fb_info_p->fb_size = val_p[1];
  }
  else
    return 0;

  // Setup successful, clear buffer and return
  memset(fb_info_p->fb_pointer, 0, fb_info_p->fb_size);
  return 1;
}

unsigned int vc_get_firmware_rev(void)
{
  int result;
  int i = 0;
  unsigned int * val_p;

  buffer[i++] = 0; // Set size later
  buffer[i++] = REQUEST_CODE;

  buffer[i++] = TAG_FIRMWARE_REV;
  buffer[i++] = 4;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = 0;

  buffer[i++] = TAG_END;
  buffer[0] = i * sizeof(int);

  // Write request and read back result
  mailbox_write((int)&buffer[0], MBOX_CHAN_VC_IF);
  result = mailbox_read(MBOX_CHAN_VC_IF);
  if (0 == result)
    return 0;

  // Verify tag value
  val_p = get_tag(buffer, TAG_FIRMWARE_REV);
  if (val_p)
  {
    TRACE_DEBUG("Firmware rev 0x%08x\r\n", val_p[0]);
  }
  else
    return 0;

  return val_p[0];
}

unsigned int vc_get_temp(void)
{
  int result;
  int i = 0;
  unsigned int * val_p;

  buffer[i++] = 0; // Set size later
  buffer[i++] = REQUEST_CODE;

  buffer[i++] = TAG_TEMPERATURE;
  buffer[i++] = 8;
  buffer[i++] = REQUEST_CODE;
  buffer[i++] = TEMPERATURE_ID;
  buffer[i++] = 0;

  buffer[i++] = TAG_END;
  buffer[0] = i * sizeof(int);

  // Write request and read back result
  mailbox_write((int)&buffer[0], MBOX_CHAN_VC_IF);
  result = mailbox_read(MBOX_CHAN_VC_IF);
  if (0 == result)
    return 0;

  // Verify tag value
  val_p = get_tag(buffer, TAG_TEMPERATURE);
  if (val_p)
  {
    TRACE_DEBUG("Temperature id %u: %u\r\n", val_p[0], val_p[1]);
  }
  else
    return 0;

  return val_p[1];
}
