#include <stdio.h>
#include <stdint.h>
#include "../ubign.h"

#define FIB_MAX 100

/* Returns one plus the index of the most significant 1-bit of n */
#define flsll(n)    (64 - __builtin_clzll(n))

/* Calculating fibonacci numbers by fast doubling */
/* f(2k) = f(k) * [2 * f(k+1) - f(k)]*/
/* f(2k+1) = f(k)^2 + f(k+1)^2 */
static ubn_t fib_sequence(long long n)
{
    ubn_t a, b, c, d;

    if (!n) {
        ubn_from_extend(&a, (ubn_b_extend_t) 0);
        return a;
    }

    if (n == 1) {
        ubn_from_extend(&a, (ubn_b_extend_t) 1);
        return a;
    }

    /* Starting from f(1), skip the most significant 1-bit */
    int i = (flsll(n) - 1) - 1;

    ubn_from_extend(&a, (ubn_b_extend_t) 1); /* f(1) = 1 */
    ubn_from_extend(&b, (ubn_b_extend_t) 1); /* f(2) = 1 */

    for (int mask = 1 << i; mask; mask >>= 1) {
        ubn_t tmp1, tmp2, tmp3;

        ubn_from_extend(&tmp1, (ubn_b_extend_t) 2); /* tmp1 = 2 */
        ubn_mul(&tmp1, &b, &tmp2);                  /* tmp2 = 2 * b */
        ubn_sub(&tmp2, &a, &tmp3);                  /* tmp3 = 2 * b - a */
        ubn_mul(&a, &tmp3, &c);                     /* c = a * (2 * b - a) */
        
        ubn_mul(&a, &a, &tmp1);                     /* tmp1 = a * a */
        ubn_mul(&b, &b, &tmp2);                     /* tmp2 = b * b */
        ubn_add(&tmp1, &tmp2, &d);                  /* d = a * a + b * b */

        if (n & mask) {
            a = d;
            ubn_add(&c, &d, &b);                    /* f(2k+2) = f(2k) + f(2k+1) = c + d */
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

int main()
{
    char buf[UBN_STR_SIZE];

    for (long long i = 0; i < FIB_MAX; i++) {
        ubn_t fib = fib_sequence(i);
        ubn_to_str(&fib, buf);
        printf("f(%lld) = %s\n", i, buf);
    }

    return 0;
}
