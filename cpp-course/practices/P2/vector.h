//
// Created by operasfantom on 12.05.18.
//

#ifndef P2_VECTOR_H
#define P2_VECTOR_H


#include <cstdio>
#include <cstring>

constexpr size_t operator "" _sz(unsigned long long n) {
    return n;
}

template<typename T>
class vector {
    size_t sz = 0, cap = 0;
    T *arr = nullptr;

    void change_capacity(size_t);

    void construct(size_t i) {
        new(arr + i) T();
    }

    /*void destruct(size_t i){
        arr[i].~T();
    }*/

    template<typename U = T>
    typename std::enable_if<std::is_trivially_destructible<U>::value>::type destruct(size_t i) {}

    template<typename U = T>
    typename std::enable_if<!std::is_trivially_destructible<U>::value>::type destruct(size_t i) {
        arr[i].~T();
    }

    template<typename U = T>
    typename std::enable_if<std::is_trivially_copyable<U>::value>::type move_data(T *d, T *s, size_t n);

    template<typename U = T>
    typename std::enable_if<!std::is_trivially_copyable<U>::value>::type move_data(T *d, T *s, size_t n);

    template<typename It>
    void vector_impl(It first, It last, std::random_access_iterator_tag);

    template<typename It>
    void vector_impl(It first, It last, std::forward_iterator_tag);

public:
    typedef T *iterator;
    typedef T *const const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector() = default;

    explicit vector(size_t);

    vector(vector<T> const &other);

    template<typename It>
    vector(It first, It last);

    ~vector();

    vector<T> &operator=(vector const &);

    void push_back(T const &);

    void pop_back() noexcept(~T());

    T &back();

    T const &back() const;

    T &operator[](size_t);

    T const &operator[](size_t) const;

    size_t size() const;

    T *data();

    T const *data() const;

    bool empty() const;

    void reserve(size_t);

    size_t capacity() const;

    void shrink_to_fit();

    void clear() noexcept;

    iterator insert(const_iterator, T const &value);

    iterator erase(const_iterator);

    iterator erase(const_iterator, const_iterator);

    void resize(size_t, T = T());

    void assign(size_t, T value = T());

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

    reverse_iterator rbegin();

    reverse_iterator rend();

    const_reverse_iterator rbegin() const;

    const_reverse_iterator rend() const;

    template<typename S>
    friend void swap(vector<S> &a, vector<S> &b);
};

#include "vector.ipp"

#endif //P2_VECTOR_H
