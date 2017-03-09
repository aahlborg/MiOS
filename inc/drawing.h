// Functions for drawing on a framebuffer

#ifndef DRAWING_H
#define DRAWING_H

// Create 8-bit RGB color
// 3 R, 3 G, 2 B
#define RGB8(r, g, b) (unsigned char)( \
          ((r) & 0x7) << 5 | \
          ((g) & 0x7) << 2 | \
          ((b) & 0x3))

// Create 16-bit RGB color
// 5 R, 6 G, 5 B
#define RGB16(r, g, b) (unsigned short)( \
          ((r) & 0x1f) << 11 | \
          ((g) & 0x3f) << 5 | \
          ((b) & 0x1f))

// Create 32-bit RGB color
// 8 A, 8 R, 8 G, 8 B
#define RGB32(r, g, b) (unsigned int)( \
          0xff000000 | \
          ((r) & 0xff) << 16 | \
          ((g) & 0xff) << 8 | \
          ((b) & 0xff))

// Forward declaration
struct framebuffer_info;

// Draw a pixel
void set_pixel(struct framebuffer_info * fb_info,
               int x, int y, int color);

// Draw a line
void draw_line(struct framebuffer_info * fb_info,
               int x0, int y0, int x1, int y1, int color);

#endif
