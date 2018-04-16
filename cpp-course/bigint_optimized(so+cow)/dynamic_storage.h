#ifndef BIGINT_DYNAMIC_STORAGE_H
#define BIGINT_DYNAMIC_STORAGE_H


#include <cstdio>
#include <cstdlib>
#include "constants.h"
#include <vector>
#include <iostream>
#include <cstring>

template<typename T>
struct dynamic_storage {
    size_t ref_counter;
    size_t size;
    size_t capacity;

    T *data;

    dynamic_storage();

    explicit dynamic_storage(T val);

    ~dynamic_storage();

    dynamic_storage<T> &operator=(dynamic_storage<T> const &other);

    dynamic_storage(dynamic_storage const &other);

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

    void change_capacity(size_t);

};

#include "dynamic_storage.hpp"

#endif //BIGINT_DYNAMIC_STORAGE_H
