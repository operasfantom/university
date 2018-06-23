#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <functional>
#include "big_data.h"

struct big_integer {
    typedef unsigned long long word_t;

    big_integer() noexcept;

    big_integer(big_integer const &other) noexcept;

    big_integer(int) noexcept;

    big_integer(word_t) noexcept;

    big_integer(std::string const &str);

    ~big_integer();

    big_integer &operator=(big_integer const &other) noexcept;

    big_integer &operator+=(big_integer const &rhs);

    big_integer &operator-=(big_integer const &rhs);

    big_integer &operator*=(big_integer const &rhs);

    big_integer &operator/=(big_integer const &rhs);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &rhs);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &rhs);

    big_integer &operator<<=(int rhs);

    big_integer &operator>>=(int rhs);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &a);

    friend big_integer absolute(big_integer const &a);

private:
    bool sign;

    bool type;

    union {
        big_data<word_t> list;

        word_t number = 0;
    };

    void pop_leading_zeros();

    word_t at(size_t) const;

    size_t size() const;

    word_t zero() const;

    void align(big_integer const &);

    void apply_bitwise_operation(big_integer const &, std::function<word_t(const word_t &, const word_t &)>);

    bool get_significant_bit(word_t back);

    void reserve(size_t);

    void change_sign();

    void transform_to_big_object();

    void transform_to_small_object();

    bool is_big_object() const;

    bool is_small_object() const;

    word_t &operator[](size_t i);

    word_t operator[](size_t i) const;

    void push_back(size_t);

    void pop_back();

    word_t back() const;

    void reverse();

    void resize(size_t);

    void reserve(size_t, word_t);

    void change_type();

    size_t size();

    void reallocate();
};

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer a, big_integer const &b);

big_integer operator/(big_integer a, big_integer const &b);

big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);

big_integer operator|(big_integer a, big_integer const &b);

big_integer operator^(big_integer a, big_integer const &b);

big_integer operator<<(big_integer a, int b);

big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const &a, big_integer const &b);

bool operator!=(big_integer const &a, big_integer const &b);

bool operator<(big_integer const &a, big_integer const &b);

bool operator>(big_integer const &a, big_integer const &b);

bool operator<=(big_integer const &a, big_integer const &b);

bool operator>=(big_integer const &a, big_integer const &b);

std::string to_string(big_integer const &a);

std::ostream &operator<<(std::ostream &s, big_integer const &a);

#endif // BIG_INTEGER_H
