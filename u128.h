#ifndef __U128_H
#define __U128_H

#include <stdint.h>

typedef struct _uint128_t   uint128_t;

struct _uint128_t {
    uint64_t lo, hi;
};

uint128_t mul64to128(uint64_t a, uint64_t b);

#endif
