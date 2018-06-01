//
// Created by operasfantom on 01.06.18.
//

#include <fstream>
#include "bit_container.h"
#include "binary_io.h"

bool bit_container::extract_bit(size_t block, size_t pos) const {
    return (vector::at(block) >> pos) & 1;
}

bool bit_container::exists_last_block()const {
    return sz % BLOCK_SIZE != 0;
}

size_t bit_container::get_number_of_block(size_t i) const {
    return i / BLOCK_SIZE;
}

size_t bit_container::get_position_in_block(size_t i) const {
    return i % BLOCK_SIZE;
}

//---

void bit_container::set_bit(size_t i, bool value) {
    if (value) {
        at(get_number_of_block(i)) |= (1u << get_position_in_block(i));
    } else {
        at(get_number_of_block(i)) &= ~(1u << get_position_in_block(i));
    }
}

bool bit_container::get_bit(size_t i) const {
    return extract_bit(get_number_of_block(i), get_position_in_block(i));
}

char bit_container::get_block(size_t i) const {
    return vector::at(i);
}

void bit_container::drop() {
    if (exists_last_block()) {
        vector::erase(begin(), end() - 1);
    } else {
        vector::clear();
    }
    sz %= BLOCK_SIZE;
}

bit_container &bit_container::operator+=(bit_container const &other) {
    for (size_t i = 0; i < other.size(); ++i) {
        push_back(other.get_bit(i));
    }
    return *this;
}

size_t bit_container::blocks_count()const {
    return vector::size();

}

void bit_container::push_back(bool x) {
    if (get_number_of_block(sz) >= vector::size()) {
        vector::push_back(0);
    }
    set_bit(sz, x);
    ++sz;
}

size_t bit_container::size() const {
    return sz;
}

void bit_container::pop_back() {
    --sz;
    if (!exists_last_block()){
        vector::pop_back();
    }
}

bit_container::bit_container(size_t n) : sz(n) {
    vector::resize((sz + (BLOCK_SIZE - 1)) / BLOCK_SIZE);
}

bit_container::bit_container() = default;
