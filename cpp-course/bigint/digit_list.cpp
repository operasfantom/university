#include <algorithm>
#include "digit_list.h"

size_t digit_list::size() const {
    return list.size();
}

void digit_list::push_back(size_t val) {
    list.push_back(val);
}

void digit_list::pop_back() {
    list.pop_back();
}

word_t digit_list::back() const {
    return list.back();
}

void digit_list::reverse() {
    std::reverse(list.begin(), list.end());
}

void digit_list::resize(size_t n) {
    list.resize(n);
}

void digit_list::resize(size_t n, word_t val) {
    list.resize(n, val);
}

