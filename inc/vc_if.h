// Interface for interacting with the Videocore GPU
// Provides functions for setting up a framebuffer
// and reading firmware revision and temperature.

#ifndef VC_IF_H
#define VC_IF_H

// Frame buffer descriptor
struct framebuffer_info {
  unsigned int width;
  unsigned int height;
  unsigned int bit_depth;
  void * fb_pointer;
  unsigned int fb_size;
  unsigned int pitch;
};

// Set up frame buffer in Videocore
int vc_init_framebuffer(struct framebuffer_info * fb_info_p);

// Read frameware revision from Videocore
unsigned int vc_get_firmware_rev(void);

// Get chip temperature from Videocore
unsigned int vc_get_temp(void);

#endif
