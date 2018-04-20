#ifndef BIGINT_BIG_DATA_H
#define BIGINT_BIG_DATA_H


#include <cstdio>
#include <cstdlib>
#include "constants.h"
#include <vector>
#include <iostream>
#include <cstring>
#include "dynamic_storage.h"

template<typename T>
struct big_data {
    size_t size = 1;

    dynamic_storage<T> *storage;

    big_data();

    explicit big_data(T val);

    ~big_data();

    big_data<T> &operator=(big_data<T> const &other);

    big_data(big_data const &other);

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

    void reserve(size_t, T);

    template<typename S>
    friend bool operator==(big_data<S> const &a, big_data<S> const &b);

    template<typename S>
    friend bool operator<(big_data<S> const &a, big_data<S> const &b);

    template<typename S>
    friend bool operator>(big_data<S> const &a, big_data<S> const &b);

    void change_capacity(size_t);

    void reallocate();
};

#include "big_data.hpp"

#endif //BIGINT_BIG_DATA_H
