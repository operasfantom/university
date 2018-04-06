#ifndef BIGINT_DIGIT_LIST_H
#define BIGINT_DIGIT_LIST_H

#include <vector>
#include "constants.h"

struct digit_list {
    std::vector<word_t> list;

    digit_list() { list = {0}; }

    digit_list(digit_list const &other) { list = other.list; }

    explicit digit_list(word_t const &a) {
        list = {static_cast<word_t>(a)};
    };

    ~digit_list() = default;

    word_t &operator[](size_t i) { return list[i]; }

    word_t operator[](size_t i) const { return list[i]; };

    size_t size() const;

    void push_back(size_t);

    void pop_back();

    word_t back() const;

    void reverse();

    void resize(size_t);

    void resize(size_t, word_t);
};

#endif //BIGINT_DIGIT_LIST_H
