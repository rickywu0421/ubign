#include <stdint.h>
#include "u128.h"

#define MASK_32b    0xffffffff

uint128_t mul64to128(uint64_t a, uint64_t b)
{
    uint128_t bn;

    uint64_t xk = (a >> 32) * (b & MASK_32b);
    uint64_t xz = (a >> 32) * (b >> 32);
    uint64_t yk = (a & MASK_32b) * (b & MASK_32b);
    uint64_t yz = (a & MASK_32b) * (b >> 32);
    
    uint64_t tmp = (xk & MASK_32b) + (yz & MASK_32b);
    uint64_t carry = (tmp + (yk >> 32)) >> 32;

    bn.lo = (tmp << 32) + yk;
    bn.hi = xz + ((xk >> 32) + (yz >> 32)) + carry;

    return bn;
}