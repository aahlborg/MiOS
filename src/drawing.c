#include <drawing.h>
#include <vc_if.h>

void set_pixel(struct framebuffer_info * fb_info,
               int x, int y, int color)
{
  const int width = fb_info->width;
  const int height = fb_info->height;
  const int bitDepth = fb_info->bit_depth;

  // Don't draw outside the framebuffer
  if (x >= width || y >= height)
  {
    return;
  }

  // Switch addressing based on color depth
  if (8 == bitDepth)
  {
    unsigned char * fb = (unsigned char *)fb_info->fb_pointer;
    fb[x + y * width] = (unsigned char)color;
  }
  else if (16 == bitDepth)
  {
    unsigned short * fb = (unsigned short *)fb_info->fb_pointer;
    fb[x + y * width] = (unsigned short)color;
  }
  else
  {
    unsigned int * fb = (unsigned int *)fb_info->fb_pointer;
    fb[x + y * width] = (unsigned int)color;
  }
}

void draw_line(struct framebuffer_info * fb_info,
               int x0, int y0, int x1, int y1, int color)
{
  int x;
  int y;
  int deltax;
  int deltay;
  int stepx;
  int stepy;
  int error;

  // Bresenham's Line Algorithm
  if (x1 > x0)
  {
    deltax = x1 - x0;
    stepx = 1;
  }
  else
  {
    deltax = x0 - x1;
    stepx = -1;
  }

  if (y1 > y0)
  {
    deltay = y1 - y0;
    stepy = 1;
  }
  else
  {
    deltay = y0 - y1;
    stepy = -1;
  }

  x = x0;
  y = y0;

  error = deltax - deltay;
  while ((x != x1 + stepx) && (y != y1 + stepy))
  {
    set_pixel(fb_info, x, y, color);
    if (2 * error >= -deltay)
    {
      x += stepx;
      error = error - deltay;
    }
    if (2 * error <= deltax)
    {
      y += stepy;
      error = error + deltax;
    }
  }
}
