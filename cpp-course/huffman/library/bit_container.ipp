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
//    return i / BLOCK_SIZE;
    return i >> BLOCK_LOG2;
}

template<typename W>
size_t bit_container<W>::get_position_in_block(size_t i) const {
//    return i % BLOCK_SIZE;
    return i & BLOCK_MASK;
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
void bit_container<W>::push_back(bool x) {//only small container
    if (std::vector<W>::empty()) {
        std::vector<W>::push_back(0);
    }
    if (x) {
        std::vector<W>::operator[](0) |= (1ull << sz);
    } else {
        std::vector<W>::operator[](0) &= ~(1ull << sz);
    }
    ++sz;
}

template<typename W>
void bit_container<W>::wide_push_back(bool x) {
    if (get_number_of_block(sz) >= (std::vector<W>::size())) {
        std::vector<W>::push_back(0);
    }
    set_bit(sz, x);
    ++sz;
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
//    std::vector<W>::resize(sz / BLOCK_SIZE + 1);
    std::vector<W>::resize((sz >> BLOCK_LOG2) + 1);
}

template<typename W>
size_t bit_container<W>::char_blocks_count() const {
    return (size() + 7) / 8;
}

template<typename W>
bit_container<W> bit_container<W>::pop() {
//    bit_container result(sz % BLOCK_SIZE);
    bit_container result(sz & BLOCK_MASK);
    result[0] = get_block(get_number_of_block(sz - 1));
//    sz -= sz % BLOCK_SIZE;
    sz -= (sz & BLOCK_MASK);
    return result;
}

template<typename W>
bit_container<W> &bit_container<W>::operator+=(const bit_container<W> &other) {
    if (size() == 0){
        return *this = other;
    }
    if (get_number_of_block(sz + other.size()) >= (std::vector<W>::size())) {
        std::vector<W>::push_back(0);
    }
    size_t last_block = get_number_of_block(sz - 1);
//    if (sz % BLOCK_SIZE == 0) {
    if ((sz & BLOCK_MASK) == 0) {
        std::vector<W>::operator[](last_block + 1) = other[0];
    } else {
//        size_t shift = sz % BLOCK_SIZE;
//        size_t inv_shift = BLOCK_SIZE - shift;
        size_t shift = sz & BLOCK_MASK;
        size_t inv_shift = BLOCK_SIZE - shift;
        std::vector<W>::operator[](last_block) &= (~0ull >> inv_shift);
        std::vector<W>::operator[](last_block) |= other[0] << shift;
        if (other.size() > inv_shift)
            std::vector<W>::operator[](last_block + 1) = other[0] >> inv_shift;
    }
    sz += other.size();
    return *this;
}


#endif //HUFFMAN_BIT_CONTAINER_IPP