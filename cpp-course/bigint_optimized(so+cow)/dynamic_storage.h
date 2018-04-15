#ifndef BIGINT_DYNAMIC_STORAGE_H
#define BIGINT_DYNAMIC_STORAGE_H


#include <cstdio>
#include <cstdlib>
#include "constants.h"
#include <vector>
#include <iostream>

template<typename T>
struct dynamic_storage {
    size_t ref_counter;
    size_t size;
    size_t capacity;

    T *data;

    dynamic_storage() {
        ref_counter = 1;
        size = 1;
        capacity = INITIAL_CAPACITY;
        data = (T *) malloc(capacity * sizeof(T));

        data[0] = 0;
    }

    explicit dynamic_storage(T val) {
        /**this = dynamic_storage();*/
        ref_counter = 1;
        size = 1;
        capacity = INITIAL_CAPACITY;
        data = (T *) malloc(capacity * sizeof(T));

        data[0] = val;
    }

    ~dynamic_storage() {
        //free(data);
    }

    dynamic_storage &operator=(dynamic_storage const &other) = default;

    void push_back(T val);

    void pop_back();

    const T *begin() const;

    const T *end() const;

    const T *rbegin() const;

    T *begin();

    T *end();

    T *rbegin();

    T back() const;

    T operator[](size_t) const;

    T &operator[](size_t);

    void erase(size_t first, size_t last);

    void resize(size_t);

    void reserve(size_t n, T);

    template<typename S>
    friend bool operator==(dynamic_storage<S> const &a, dynamic_storage<S> const &b);

    template<typename S>
    friend bool operator<(dynamic_storage<S> const &a, dynamic_storage<S> const &b);

    template<typename S>
    friend bool operator>(dynamic_storage<S> const &a, dynamic_storage<S> const &b);
};

#include "dynamic_storage.hpp"

#endif //BIGINT_DYNAMIC_STORAGE_H
