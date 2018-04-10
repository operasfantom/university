#include "arithmetic.h"

word_t simple_add(word_t a, word_t b, word_t &cf) { // cf in {0, 1}
    __uint128_t t = (__uint128_t) 0 + (__uint128_t) a + (__uint128_t) b + (__uint128_t) cf;
    cf = static_cast<word_t>(t >> SIZEOF_WORD_T);
    return static_cast<word_t>(t);
}

word_t simple_mul(word_t a, word_t b, word_t &cf) {
    __uint128_t t = (__uint128_t) 1 * (__uint128_t) a * (__uint128_t) b;
    cf = static_cast<word_t>(t >> SIZEOF_WORD_T);
    return static_cast<word_t>(t);
}

