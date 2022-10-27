#include "xorshift.h"

uint32_t x;

void set_xor32_seed(uint32_t seed) {
  x = seed;
}

uint32_t xorshift32() {
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  return x;
}
