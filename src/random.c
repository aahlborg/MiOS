#include <random.h>

// Initial seed
static unsigned int w = 0x37FB4D20;
static unsigned int z = 0xA7B35C57;

void rnd_seed(unsigned int seed)
{
  // Just mix value into w and z
  w = w ^ seed;
  z = z ^ (seed << 16 | seed >> 16);
}

unsigned int random(void)
{
  // Pseudo-random generation
  w = 18000 * (w & 0xffff) + (w >> 16);
  z = 36969 * (z & 0xffff) + (z >> 16);
  return w + (z << 16);
}
