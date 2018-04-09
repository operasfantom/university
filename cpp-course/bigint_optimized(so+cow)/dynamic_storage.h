#ifndef BIGINT_DYNAMIC_STORAGE_H
#define BIGINT_DYNAMIC_STORAGE_H


#include <cstdio>
#include <cstdlib>
#include "constants.h"
#include <vector>

template<typename T>
struct dynamic_storage {
    size_t ref_counter;
    size_t size;
    size_t capacity;

    T data[];

    dynamic_storage() {
        ref_counter = 1;
        size = 1;
        capacity = INITIAL_CAPACITY;
        //data = realloc(INITIAL_CAPACITY * sizeof(T));//TODO
    }

    void push_back(T val);

    void pop_back();

    const T *begin() const;

    const T *end() const;

    const T *rbegin() const;

    const T *rend() const;

    T *begin();

    T *end();

    T *rbegin();

    T *rend();

    T back() const;

    T operator[](size_t) const;

    T &operator[](size_t);

    void erase(size_t first, size_t last);

    void resize(size_t);

    void resize(size_t, word_t);

    template <typename S>
    friend bool operator==(dynamic_storage<S> const &a, dynamic_storage<S> const &b);

    void reserve(size_t n);
};

#include "dynamic_storage.hpp"

#endif //BIGINT_DYNAMIC_STORAGE_H
