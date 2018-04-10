#include <algorithm>
#include <cassert>
#include <cstring>
#include "data_union.h"

data_union::data_union() {
    type = SMALL;
    number = 0;
}

void data_union::push_back(size_t val) {
    if (is_small_object()) {
        from_small_to_big();
    } else {
        list->push_back(val);
    }
}

void data_union::from_small_to_big() {
    list.reset(new dynamic_storage<word_t>());
}

void data_union::pop_back() {
    list->pop_back();
}

word_t data_union::back() const {
    return list->back();
}

void data_union::reverse() {
    std::reverse(list->begin(), list->end());
}

void data_union::resize(size_t n) {
    list->resize(n);
}

void data_union::resize(size_t n, word_t val) {
    list->resize(n, val);
}

bool data_union::is_small_object() const {
    return type == SMALL;
}

word_t &data_union::operator[](size_t i) {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        (*list)[i];
    }
}

word_t data_union::operator[](size_t i) const {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        (*list)[i];
    }
}

data_union::data_union(word_t const &a) {
    type = SMALL;
    number = a;
}

data_union data_union::operator=(data_union const &other) {
    type = other.type;
    number = other.number;
}

data_union::~data_union() {}

bool operator==(data_union const &a, data_union const &b) {
    if (a.type == SMALL && b.type == SMALL) {
        return a.number == b.number;
    } else if (a.type == BIG && a.type == BIG) {
        return memcmp(a.list->begin(), b.list->begin(), a.list->size * sizeof(word_t)) == 0;
    } else {
        return a.type == SMALL;
    }
}

bool operator<(data_union const &a, data_union const &b) {
    return false;
}

bool operator>(data_union const &a, data_union const &b) {
    return false;
}
