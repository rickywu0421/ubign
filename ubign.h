#ifndef __UBIGN_H
#define __UBIGN_H

#include <stdint.h>
#include <string.h>

typedef uint32_t        ubn_b_t;
typedef uint64_t        ubn_b_extend_t;
typedef struct _ubn     ubn_t;

#define BITS_PER_BYTE           8
#define UBN_BLOCK_SIZE          sizeof(ubn_b_t)
#define UBN_BLOCK_EXTEND_SIZE   sizeof(ubn_b_extend_t)
#define UBN_FULL_SIZE           128
#define UBN_BLOCK_MAX           (ubn_b_t) -1
#define UBN_ARRAY_SIZE          (UBN_FULL_SIZE / UBN_BLOCK_SIZE)

/** UBN_STR_SIZE - the length of string of base 10 unsigned big number
 *  To determine the size, we need to calculate
 *  log(2 ^ ((UBN_BLOCK_SIZE * BITS_PER_BYTE) * UBN_ARRAY_SIZE))
 *  which is 308.25..., we ceil it and plus 1 for null terminator 
*/
#define UBN_STR_SIZE            310

struct _ubn {
    ubn_b_t arr[UBN_ARRAY_SIZE];
};

#define ubn_init(ubn, n) \
    memset(ubn, n, sizeof(ubn_b_t) * UBN_ARRAY_SIZE)

static inline void ubn_from_extend(ubn_t *ubn, ubn_b_extend_t tmp)
{
    ubn_init(ubn, 0);
    ubn->arr[0] = tmp;
    ubn->arr[1] = tmp >> (BITS_PER_BYTE * UBN_BLOCK_SIZE);
}

void ubn_add(ubn_t *a, ubn_t *b, ubn_t *c);
void ubn_sub(ubn_t *a, ubn_t *b, ubn_t *c);
void ubn_lshift_b(ubn_t *ubn, int block);
void ubn_mul(ubn_t *a, ubn_t *b, ubn_t *c);
void ubn_to_str(ubn_t *ubn, char *buf);

#endif
