#include <string.h>
#include "ubign.h"

void ubn_add(ubn_t *a, ubn_t *b, ubn_t *c)
{
    int carry = 0;
    for (int i = 0; i < UBN_ARRAY_SIZE; i++) {
        c->arr[i] = a->arr[i] + b->arr[i] + carry;
        carry = (c->arr[i] < a->arr[i]);
    }
}

void ubn_sub(ubn_t *a, ubn_t *b, ubn_t *c)
{
    int borrow = 0;
    for (int i = 0; i < UBN_ARRAY_SIZE; i++) {
        c->arr[i] = a->arr[i] - b->arr[i] - borrow;
        borrow = (c->arr[i] > a->arr[i]);
    }
}

void ubn_lshift_b(ubn_t *ubn, int block)
{
    for (int i = UBN_ARRAY_SIZE; i >= block; i--) {
        ubn->arr[i] = ubn->arr[i - block];
    }

    /* Zero padding */ 
    memset(ubn->arr, 0, sizeof(ubn_b_t) * block);
}

void ubn_mul(ubn_t *a, ubn_t *b, ubn_t *c)
{
    ubn_init(c, 0);

    for (int i = 0; i < UBN_ARRAY_SIZE; i++) {
        for (int j = 0; j < UBN_ARRAY_SIZE; j++) {
            if ((i + j) < UBN_ARRAY_SIZE) {
                ubn_t ubn;
                ubn_b_extend_t tmp = a->arr[i] * b->arr[i];
                ubn_from_extend(&ubn, tmp);
                ubn_lshift_b(&ubn, i + j);
                ubn_add(&ubn, c, c);
            }
        }
    }
}

void ubn_to_str(ubn_t *ubn, char *buf)
{
    ubn_init(ubn, '0');

    /* Skip zero block */
    int index;
    for (index = UBN_ARRAY_SIZE - 1; !ubn->arr[index]; index--)
        ;

    for (; index >= 0; index--) {
        for (ubn_b_extend_t mask = 1 << (UBN_BLOCK_SIZE - 1); 
            mask; mask >>= 1) {
            int carry = ((ubn->arr[index] & mask) != 0);

            for (int i = UBN_STR_SIZE - 2; i >= 0; i--) {
                buf[i] += buf[i] + carry - '0';

                if (buf[i] > '9') {
                    carry = 1;
                    buf[i] -= 10;
                }
            }
        }
    }

    /* Eliminate leading zeros in buf */
    int offset;
    for (offset = 0; 
        (offset < UBN_STR_SIZE - 2) && (ubn->arr[offset] == '0'); 
         offset++)
        ;
    int i;
    for (i = 0; i < (UBN_STR_SIZE - 1 - offset); i++)
        ubn->arr[i] = ubn->arr[i + offset];
    ubn->arr[i] = '\0';
}

