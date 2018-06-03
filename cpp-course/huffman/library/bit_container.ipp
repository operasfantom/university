#ifndef HUFFMAN_BIT_CONTAINER_IPP
#define HUFFMAN_BIT_CONTAINER_IPP

#include <fstream>
#include <cassert>
#include <limits>
#include "bit_container.h"

template<typename W>
bool bit_container<W>::extract_bit(size_t block, size_t pos) const {
    return (std::vector<W>::operator[](block) >> pos) & 1;
}

template<typename W>
size_t bit_container<W>::get_number_of_block(size_t i) const {
    return i / BLOCK_SIZE;
}

template<typename W>
size_t bit_container<W>::get_position_in_block(size_t i) const {
    return i % BLOCK_SIZE;
}

//---

template<typename W>
void bit_container<W>::set_bit(size_t i, bool value) {
    if (value) {
        std::vector<W>::operator[](get_number_of_block(i)) |= (1ull << get_position_in_block(i));
    } else {
        std::vector<W>::operator[](get_number_of_block(i)) &= ~(1ull << get_position_in_block(i));
    }
}

template<typename W>
bool bit_container<W>::get_bit(size_t i) const {
    return extract_bit(get_number_of_block(i), get_position_in_block(i));
}

template<typename W>
W bit_container<W>::get_block(size_t i) const {
    return std::vector<W>::operator[](i);
}

template<typename W>
void bit_container<W>::drop(size_t x) {
    std::vector<W>::operator[](0) = x;
    sz = BLOCK_SIZE;
}

template<typename W>
void bit_container<W>::push_back(bool x) {
    if (get_number_of_block(sz) >= (std::vector<W>::size())) {
        std::vector<W>::push_back(0);
    }
    set_bit(sz, x);
    ++sz;
}

template<typename W>
bit_container<W> &bit_container<W>::operator+=(bit_container<W> const &other) {
    for (size_t i = 0; i < other.size(); ++i) {
        push_back(other.get_bit(i));
    }
    return *this;
}

template<typename W>
size_t bit_container<W>::size() const {
    return sz;
}

template<typename W>
void bit_container<W>::pop_back() {
    --sz;
}

template<typename W>
bit_container<W>::bit_container(size_t n) : bit_container() {
    sz = n;
    std::vector<W>::resize(sz / BLOCK_SIZE + 1);
}

template<typename W>
size_t bit_container<W>::char_blocks_count() const {
    return (size() + 7) / 8;
}

template<typename W>
bit_container<W> bit_container<W>::pop() {
    bit_container result(sz % BLOCK_SIZE);
    result[0] = get_block(get_number_of_block(sz - 1));
    sz /= BLOCK_SIZE;
    sz *= BLOCK_SIZE;
    return result;
}

/*template<typename W>
bit_container<W>::bit_container() {
    BLOCK_SIZE = (size_t) std::numeric_limits<W>::digits;
};*/

#endif //HUFFMAN_BIT_CONTAINER_IPP