#include "arithmetic.h"

word_t simple_add(word_t a, word_t b, word_t &cf) { // cf in {0, 1}
    /*__uint128_t t = (__uint128_t) 0 + (__uint128_t) a + (__uint128_t) b + (__uint128_t) cf;
    cf = static_cast<word_t>(t >> SIZEOF_WORD_T);
    return static_cast<word_t>(t);*/
    word_t result = a + b + cf;
    if ((b == WORD_MAX && (a != 0 || cf != 0)) || (a > WORD_MAX - b - cf)) {
        cf = 1;
    } else {
        cf = 0;
    }
    return result;
}

inline word_t get_high_half(word_t const &x) {
    return x >> HALF_SIZE;
}

inline word_t get_low_half(word_t const &x) {
    return x << HALF_SIZE >> HALF_SIZE;
}

word_t simple_mul(word_t a, word_t b, word_t &cf) {
    /*__uint128_t t = (__uint128_t) 1 * (__uint128_t) a * (__uint128_t) b;
    cf = static_cast<word_t>(t >> SIZEOF_WORD_T);
    return static_cast<word_t>(t);*/
    word_t a_hi = get_high_half(a);
    word_t a_low = get_low_half(a);

    word_t b_hi = get_high_half(b);
    word_t b_low = get_low_half(b);

    word_t result = a_low * b_low;
    cf = a_hi * b_hi;

    word_t carry = 0;

    word_t t = simple_add(a_low * b_hi, a_hi * b_low, carry);

    cf -= carry;
    cf = simple_add(cf, carry << HALF_SIZE, carry); // Post: carry == 0

    result = simple_add(result, get_low_half(t) << HALF_SIZE, carry);

    cf += get_high_half(t) + carry;

    return result;
}

