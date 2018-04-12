#ifndef BIGINT_DIGIT_LIST_H
#define BIGINT_DIGIT_LIST_H

#include <vector>
#include <memory>
#include "constants.h"
#include "dynamic_storage.h"

const bool SMALL = false;
const bool BIG = true;

struct data_union {
    bool type;

    union {
        std::shared_ptr<dynamic_storage<word_t> > list;

        word_t number;
    };

    data_union();

    ~data_union();

    data_union(data_union const &other) {};

    data_union operator=(data_union const &other);

    explicit data_union(word_t const &a);

    word_t &operator[](size_t i);

    word_t operator[](size_t i) const;

    void push_back(size_t);

    void pop_back();

    word_t back() const;

    void reverse();

    void resize(size_t);

    void resize(size_t, word_t);

    void from_small_to_big();

    bool is_small_object() const;

    friend bool operator==(data_union const &a, data_union const &b);

    friend bool operator<(data_union const &a, data_union const &b);
    friend bool operator>(data_union const &a, data_union const &b);
};

#endif //BIGINT_DIGIT_LIST_H