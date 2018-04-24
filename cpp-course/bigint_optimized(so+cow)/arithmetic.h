#ifndef BIGINT_ARITHMETIC_H
#define BIGINT_ARITHMETIC_H

#include "constants.h"

word_t simple_add(word_t, word_t, word_t&);

word_t simple_mul(word_t, word_t, word_t&);

word_t two_s_complement(word_t const &);

#endif //BIGINT_ARITHMETIC_H
