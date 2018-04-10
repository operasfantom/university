#ifndef BIGINT_CONSTANTS_H
#define BIGINT_CONSTANTS_H

#include <cstdio>
#include <limits>

typedef size_t word_t;

const bool PLUS = false;
const bool MINUS = true;

const word_t ZERO_PLUS = 0;
const word_t ZERO_MINUS = static_cast<const word_t>(std::numeric_limits<word_t>::max());

const size_t BLOCK_SIZE = 19;
const word_t BLOCK = static_cast<const word_t>(1e19);

const word_t WORD_MAX = std::numeric_limits<word_t >::max();

const size_t SIZEOF_WORD_T = static_cast<const size_t>(std::numeric_limits<word_t>::digits);
const size_t HALF_SIZE = SIZEOF_WORD_T / 2;

const size_t INITIAL_SIZE = 1;
const size_t INITIAL_CAPACITY = 2;
#endif //BIGINT_CONSTANTS_H
