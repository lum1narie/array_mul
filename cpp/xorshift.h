#ifndef XORSHIFT_H
#define XORSHIFT_H

#include <stdint.h>

void set_xor32_seed(uint32_t seed);
uint32_t xorshift32();

#endif
