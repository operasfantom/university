#include <algorithm>
#include <cstring>
#include "data_union.h"

data_union::data_union() {
    type = SMALL;
    number = 0;
}

void data_union::push_back(size_t val) {
    if (is_small_object()) {
        change_type();
    }
    list.push_back(val);
}

void data_union::from_small_to_big() {
    if (is_small_object()) {
        change_type();
    } else {
        assert(0);
    }
}

void data_union::pop_back() {
    list.pop_back();
}

word_t data_union::back() const {
    if (is_small_object()) {
        return number;
    } else {
        return list.back();
    }
}

void data_union::reverse() {
    if (is_small_object()) {
        return;
    }
    std::reverse(list.begin(), list.end());
}

void data_union::reserve(size_t n, word_t val) {
    if (is_small_object()) {
        if (n > 1) {
            from_small_to_big();
        } else {
            return;;
        }
    }
    list.reserve(n, val);
}

void data_union::resize(size_t n) {
    if (n > size()) {
        reserve(n, 0);
    } else {
        if (is_small_object()) {
            return;
        }
        list.resize(n);
    }
}

bool data_union::is_small_object() const {
    return type == SMALL;
}

inline bool data_union::is_big_object() const {
    return type == BIG;
}

word_t &data_union::operator[](size_t i) {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        return list[i];
    }
}

word_t data_union::operator[](size_t i) const {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        return list[i];
    }
}

data_union::data_union(word_t const &a) {
    type = SMALL;
    number = a;
}

data_union &data_union::operator=(data_union const &other) {
    if (type == other.type) {
        if (is_small_object()) { // S S
            number = other.number;
        } else { // B B
            list.~dynamic_storage();
            list = other.list;
        }
    } else {
        if (is_small_object()) { // S B
            list = other.list;
        } else { // B S
            list.~dynamic_storage();
            number = other.number;
        }
    }
    type = other.type;
    return *this;
}

data_union::~data_union() {
    if (is_big_object()) {
        list.~dynamic_storage();
    }
}

bool operator==(data_union const &a, data_union const &b) {
    if (a.is_small_object() && b.is_small_object()) {
        return a.number == b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list == b.list;
    } else {
        return a.is_small_object();
    }
}

bool operator<(data_union const &a, data_union const &b) {
    if (a.is_small_object() && b.is_small_object()) {
        return a.number < b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list < b.list;
    } else {
        return a.is_small_object();
    }
}

bool operator>(data_union const &a, data_union const &b) {
    if (a.is_small_object() && b.is_small_object()) {
        return a.number > b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list > b.list;
    } else {
        return a.is_small_object();
    }
}

void data_union::change_type() {
    if (is_small_object()) {
        list = dynamic_storage<word_t>(number);
    } else {
        word_t tmp = list[0];
        list.~dynamic_storage();
        number = tmp;
    }
    type ^= true;
}

data_union::data_union(data_union const &other) : type(other.type) {
    if (is_small_object()) {
        number = other.number;
    } else {
        //list.~dynamic_storage();
        list = other.list;
    }
}

size_t data_union::size() {
    if (is_small_object()) {
        return 1;
    } else {
        return list.size;
    }
}