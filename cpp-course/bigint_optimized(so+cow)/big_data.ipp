#ifndef BIGINT_BIG_DATA_HPP
#define BIGINT_BIG_DATA_HPP


#include <cassert>
#include <cstring>
#include "big_data.h"
#include "dynamic_storage.h"

template<typename T>
const T *big_data<T>::begin() const {
    return storage->data;
}

template<typename T>
const T *big_data<T>::end() const {
    return storage->data + size;
}

template<typename T>
const T *big_data<T>::rbegin() const {
    return storage->data + size - 1;
}

template<typename T>
T *big_data<T>::begin() {
    return storage->data;
}

template<typename T>
T *big_data<T>::end() {
    return storage->data + size;
}

template<typename T>
T *big_data<T>::rbegin() {
    return storage->data + size - 1;
}

template<typename T>
T big_data<T>::operator[](size_t i) const {
    return storage->data[i];
}

template<typename T>
T &big_data<T>::operator[](size_t i) {
    return storage->data[i];
}

template<typename T>
void big_data<T>::push_back(T val) {
    if (storage->capacity == size) {
        change_capacity(storage->capacity * 2);
    }
    storage->data[size++] = val;
}

template<typename T>
void big_data<T>::pop_back() noexcept {
    size--;
}

template<typename T>
T big_data<T>::back() const {
    assert(size > 0);
    return storage->data[size - 1];
}

template<typename T>
void big_data<T>::erase(size_t first, size_t last) noexcept {
    size_t delta = last - first;
    for (size_t i = last; i < size; i++) {
        storage[i - delta] = storage[i];
    }
    resize(size - delta);
}

template<typename T>
void big_data<T>::reserve(size_t n, T val) {
    if (n > size) {
        if (n > storage->capacity) {
            change_capacity(std::max(n, storage->capacity * 2));
        }
        for (size_t i = size; i < n; i++) {
            storage->data[i] = val;
        }
        size = n;
    }
}

template<typename T>
int reverse_memcmp(const T *end1, const T *end2, size_t n) {
    const T *a = end1, *b = end2;
    for (size_t i = 0; i < n; ++i) {
        if (*a < *b) {
            return -1;
        }
        if (*a > *b) {
            return +1;
        }
        --a, --b;
    }
    return 0;
}

template<typename S>
bool operator==(const big_data<S> &a, const big_data<S> &b) {
    if (a.size != b.size) {
        return false;
    } else {
        return reverse_memcmp(a.rbegin(), b.rbegin(), a.size) == 0;
    }
}

template<typename S>
bool operator<(const big_data<S> &a, const big_data<S> &b) {
    if (a.size != b.size) {
        return a.size < b.size;
    } else {
        return reverse_memcmp(a.rbegin(), b.rbegin(), a.size) < 0;
    }
}

template<typename S>
bool operator>(const big_data<S> &a, const big_data<S> &b) {
    if (a.size != b.size) {
        return a.size > b.size;
    } else {
        return reverse_memcmp(a.rbegin(), b.rbegin(), a.size) > 0;
    }
}

template<typename T>
void big_data<T>::resize(size_t n) noexcept {
    size = n;
}

template<typename T>
void big_data<T>::change_capacity(size_t n) {
    storage->capacity = n;
    void *new_data = realloc(storage->data, storage->capacity * sizeof(T));
    //std::cerr << "realloc:" << storage << ' ' << new_data << '\n';
    if (new_data == nullptr) {
        std::cerr << "couldn't realloc()";
        exit(0);
    } else {
        storage->data = (T *) new_data;
    }
}

template<typename T>
big_data<T>::~big_data() {
    //std::cerr << "free: " << storage << '\n';
    storage->ref_counter--;
    if (storage->ref_counter == 0) {
        delete storage;
    }
}

template<typename T>
big_data<T>::big_data() noexcept {
    storage = new (std::nothrow) dynamic_storage<T>();
    if (storage == nullptr){
        std::cerr << "couldn't create big_data()";
        exit(0);
    }
    size = INITIAL_SIZE;
    storage->capacity = INITIAL_CAPACITY;
    storage->data[0] = 0;
}

template<typename T>
big_data<T>::big_data(T val) noexcept {
    storage = new (std::nothrow) dynamic_storage<T>();
    if (storage == nullptr){
        std::cerr << "couldn't create big_data(" << val << ")";
        exit(0);
    }
    size = INITIAL_SIZE;
    storage->capacity = INITIAL_CAPACITY;
    storage->data[0] = val;
}

template<typename T>
big_data<T>::big_data(big_data const &other) noexcept : size(other.size), storage(other.storage) {
    storage->ref_counter++;
}

template<typename T>
big_data<T> &big_data<T>::operator=(const big_data<T> &other) noexcept {
    size = other.size;
    storage = other.storage;
    storage->ref_counter++;
    return *this;
}

template<typename T>
void big_data<T>::reallocate() {
    if (storage->ref_counter > 1) {
        storage->ref_counter--;
        storage = new dynamic_storage<T>(*storage);
    }
}

#endif //BIGINT_BIG_DATA_HPP
