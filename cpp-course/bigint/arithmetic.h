#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

#include <limits>

#include "digit_list.h"

const size_t SIZEOF_WORD_T = std::numeric_limits<word_t>::digits;
const size_t HALF_SIZE = SIZEOF_WORD_T / 2;

word_t simple_add(word_t, word_t, word_t&);

word_t simple_mul(word_t, word_t, word_t&);

#endif //BIGINT_ARITHMETIC_H
