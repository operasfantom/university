#ifndef P2_VECTOR_IPP
#define P2_VECTOR_IPP

#include "vector.h"
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <cstring>
#include <iterator>

template<typename T>
vector<T>::vector(size_t n) {
    change_capacity(n);
    sz = n;
}

template<typename T>
vector<T>::vector(vector<T> const &other) {
    change_capacity(other.cap);
    sz = other.sz;
    move_data(this->arr, other.arr, sz);
}

template<typename T>
template<typename It>
void vector<T>::vector_impl(It first, It last, std::random_access_iterator_tag) {
    change_capacity(last - first);
    sz = cap;
    for (It i = first; i != last; ++i) {
        construct(i - first);
        arr[i - first] = *i;
    }
}

template<typename T>
template<typename It>
void vector<T>::vector_impl(It first, It last, std::forward_iterator_tag) {
    for (It i = first; i != last; ++i) {
        push_back(*i);
    }
}

template<typename T>
template<typename It>
vector<T>::vector(It first, It last) {
    vector_impl(first, last, typename std::iterator_traits<It>::iterator_category());
}

template<typename T>
vector<T>::~vector() {
    for (size_t i = 0; i != sz; ++i) {
        destruct(i);
    }
    operator delete(arr);
}

template<typename T>
vector<T> &vector<T>::operator=(vector<T> const &other) {
    if (&other == this) {
        return *this;
    }
    vector<T> t(other);
    swap(*this, t);
    return *this;
}

template<typename T>
void vector<T>::push_back(const T &value) {
    if (cap == sz) {
        change_capacity(std::max(1_sz, cap * 2));
    }
    arr[sz++] = value;
}

template<typename T>
void vector<T>::pop_back() noexcept(~T()) {
    destruct(--sz);
}

template<typename T>
T &vector<T>::back() {
    return arr[sz];
}

template<typename T>
T const &vector<T>::back() const {
    return arr[sz];
}

template<typename T>
T &vector<T>::operator[](size_t i) {
    return arr[i];
}

template<typename T>
T const &vector<T>::operator[](size_t i) const {
    return arr[i];
}

template<typename T>
size_t vector<T>::size() const {
    return sz;
}

template<typename T>
T *vector<T>::data() {
    return arr;
}

template<typename T>
T const *vector<T>::data() const {
    return arr;
}

template<typename T>
bool vector<T>::empty() const {
    return sz == 0;
}

template<typename T>
void vector<T>::reserve(size_t n) {
    if (n > cap) {
        change_capacity(std::max(n, cap * 2));
    }
}

template<typename T>
size_t vector<T>::capacity() const {
    return cap;
}

template<typename T>
void vector<T>::shrink_to_fit() {
    change_capacity(sz);
}

template<typename T>
void vector<T>::clear() noexcept {
    sz = 0;
}

template<typename T>
typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator position, const T &value) {
    vector<int> buf(position, end());
    resize(position - begin());
    push_back(value);
    for (T &x : buf) {
        push_back(x);
    }
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator position) {
    erase(position, position + 1);
}

template<typename T>
typename vector<T>::iterator vector<T>::erase(vector::const_iterator first, vector::const_iterator last) {
    size_t answer_position = first - begin();

    move_data(first, last, end() - last);
    resize(size() - (last - first));
    return begin() + answer_position;
}

template<typename T>
void vector<T>::resize(size_t n, T value) {
    if (n == sz) {
        return;
    }
    if (n > sz) {
        if (n > cap) {
            change_capacity(std::max(n, cap * 2));
        }
        for (size_t i = sz; i != n; ++i) {
            arr[i] = value;
        }
    } else {
        change_capacity(n);
    }
    sz = n;
}

template<typename T>
void vector<T>::assign(size_t n, T value) {
    change_capacity(n);
    for (size_t i = 0; i != n; ++i) {
        construct(i);
        arr[i] = value;
    }
}

//<editor-fold desc="friend">
template<typename S>
void swap(vector<S> &a, vector<S> &b) {
    std::swap(a.sz, b.sz);
    std::swap(a.cap, b.cap);
    std::swap(a.arr, b.arr);
}
//</editor-fold>

//region private
template<typename T>
void vector<T>::change_capacity(size_t n) {
    cap = n;
    T *new_data = (T *) operator new(cap * sizeof(T));
    for (size_t i = 0; i != std::min(sz, n); ++i) {
        new_data[i] = arr[i];
        destruct(i);
    }
    free(arr);
    arr = new_data;
}

template<typename T>
typename vector<T>::iterator vector<T>::begin() {
    return arr;
}

template<typename T>
typename vector<T>::iterator vector<T>::end() {
    return arr + sz;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::begin() const {
    return arr;
}

template<typename T>
typename vector<T>::const_iterator vector<T>::end() const {
    return arr + sz;
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rbegin() {
    return reverse_iterator(end());
}

template<typename T>
typename vector<T>::reverse_iterator vector<T>::rend() {
    return reverse_iterator(begin());
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rbegin() const {
    return const_reverse_iterator(end());
}

template<typename T>
typename vector<T>::const_reverse_iterator vector<T>::rend() const {
    return const_reverse_iterator(begin());
}

template<typename T>
template<typename U>
typename std::enable_if<std::is_trivially_copyable<U>::value>::type vector<T>::move_data(T *d, T *s, size_t n) {
    memmove(d, s, n * sizeof(T));
}

template<typename T>
template<typename U>
typename std::enable_if<!std::is_trivially_copyable<U>::value>::type vector<T>::move_data(T *d, T *s, size_t n) {
    for (size_t i = 0; i != n; ++i) {
        construct(i);
        d[i] = s[i];
    }
}
//endregion


#endif //P2_VECTOR_IPP