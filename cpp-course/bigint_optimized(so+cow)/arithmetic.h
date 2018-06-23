#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

#include "constants.h"

#define word_t unsigned long long

word_t simple_add(word_t, word_t, word_t&);

word_t simple_mul(word_t, word_t, word_t&);

template <typename T>
T two_s_complement(T const & x){
    return ~x + 1;
}

#undef word_t

#endif //BIGINT_ARITHMETIC_H
