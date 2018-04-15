#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <functional>
#include "data_union.h"

struct big_integer {
    big_integer();

    big_integer(big_integer const &other);

    big_integer(int);

    big_integer(std::string const &str);

    big_integer(word_t a);

    ~big_integer();

    big_integer &operator=(big_integer const &other);

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

    data_union data;

    void pop_leading_zeros();

    word_t at(size_t) const;

    size_t size() const;

    word_t zero() const;

    void align(big_integer const &);

    void apply_logic(big_integer const &, std::function<void(word_t &, word_t)>);

    bool get_significant_bit(word_t back);

    void reserve(size_t);

    void change_sign();

    bool is_small_object()const;

    void transform_to_big_object();

    void transform_to_small_object();
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

big_integer abs(big_integer const &a);

#endif // BIG_INTEGER_H
