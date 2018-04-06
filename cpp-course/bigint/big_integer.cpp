#include "big_integer.h"
#include "arithmetic.h"

#include <algorithm>

big_integer::big_integer() : digits(), sign(PLUS) {}

big_integer::big_integer(big_integer const &other) = default;

big_integer::big_integer(int a) : digits(a), sign(a >= 0 ? PLUS : MINUS) {}

big_integer::big_integer(word_t a): digits(a), sign(PLUS) {}

big_integer::big_integer(std::string const &str) : big_integer() {
    bool new_sign = (str[0] == '-' ? MINUS : PLUS);
    sign = PLUS;
    for (size_t i = (new_sign == PLUS ? 0 : 1); i < str.length(); i += BLOCK_SIZE) {
        size_t current_block_size = std::min(BLOCK_SIZE, str.length() - i);
        std::string part = str.substr(i, current_block_size);
        if (i + BLOCK_SIZE > str.length()) {
            word_t power10 = 1;
            for (size_t j = 0; j < current_block_size; ++j) {
                power10 *= 10;
            }
            *this *= power10;
        } else {
            *this *= BLOCK;
        }
        *this += static_cast<word_t>(stoull(part));
    }

    pop_leading_zeros();

    if (new_sign == MINUS) {
        this->change_sign();
    }
}

big_integer::~big_integer() = default;

big_integer &big_integer::operator=(big_integer const &other) = default;

big_integer &big_integer::operator+=(big_integer const &rhs) {
    align(rhs);

    word_t cf = 0;
    for (size_t i = 0; i < size(); ++i) {
        digits[i] = simple_add(at(i), rhs.at(i), cf);
    }

    digits.push_back(simple_add(zero(), rhs.zero(), cf));

    bool new_sign = get_significant_bit(digits.back()) != 0 ? MINUS : PLUS;
    if (new_sign != sign) {
        sign = new_sign;
    }


    pop_leading_zeros();

    return *this;
}

big_integer &big_integer::operator-=(big_integer const &rhs) {
    *this += (-rhs);

    pop_leading_zeros();

    return *this;
}

big_integer &big_integer::operator*=(big_integer const &rhs) {
    bool result_sign = (this->sign) ^(rhs.sign);

    big_integer const *abs_a = this->sign == MINUS ? new big_integer(abs(*this)) : this;
    big_integer const *abs_b = rhs.sign == MINUS ? new big_integer(abs(rhs)) : &rhs;
    big_integer result;

    result.reserve(abs_a->size() + abs_b->size());

    for (size_t i = 0; i < abs_a->size(); ++i) {
        word_t rdx = 0, cf = 0, factor = abs_a->digits[i];
        for (size_t j = 0; j < abs_b->size(); ++j) {
            result.digits[i + j] = simple_add(result.digits[i + j], rdx, cf);
            word_t rax = simple_mul(factor, abs_b->digits[j], rdx);
            rdx += cf;
            cf = 0;
            result.digits[i + j] = simple_add(result.digits[i + j], rax, cf);
        }
        result.digits[i + abs_b->size()] = rdx + cf;
    }

    result.pop_leading_zeros();

    if (result_sign == MINUS) {
        result.change_sign();
    }

    if (abs_a != this){
        delete abs_a;
    }
    if (abs_b != &rhs){
        delete abs_b;
    }

    return *this = result;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    bool result_sign = this->sign ^rhs.sign;

    big_integer abs_a = abs(*this);
    big_integer abs_b = abs(rhs);
    big_integer result;

    result.reserve(abs_a.size());

    word_t scale;
    if (abs_b.digits.back() < WORD_MAX / 2) {
        scale = WORD_MAX / (abs_b.digits.back() + 1);
        abs_a *= scale;
        abs_b *= scale;
    }

    big_integer dividend;

    for (size_t i = abs_a.size(); i > 0; --i) { // dividend / divider = quotient
        big_integer &divider = abs_b;

        dividend <<= SIZEOF_WORD_T;
        dividend |= abs_a.digits[i - 1];

        dividend.pop_leading_zeros();

        if (dividend.size() < divider.size()) {
            result.digits.push_back(0);
            continue;
        }

        if (dividend.size() == divider.size()) {
            dividend.digits.push_back(0);
        }

        __uint128_t guess = dividend.digits.back();

        guess <<= SIZEOF_WORD_T;
        guess |= dividend.digits[dividend.size() - 2];

        guess /= static_cast<__uint128_t>(divider.digits.back());

        big_integer composition;
        composition.reserve(2);
        composition.digits[0] = static_cast<word_t>(guess);
        composition.digits[1] = static_cast<word_t>(guess >> SIZEOF_WORD_T);

        composition *= divider;

        //size_t steps = 0;
        while (guess > 0 && composition > dividend) {
            --guess;
            composition -= divider;

            //steps++;
        }

        //assert(steps <= 2);

        result.digits.push_back(static_cast<size_t>(guess));

        dividend -= composition;
    }

    result.digits.reverse();

    result.pop_leading_zeros();

    if (result_sign == MINUS) {
        result.change_sign();
    }

    return *this = result;
}

big_integer &big_integer::operator%=(big_integer const &rhs) {
    return *this -= *this / rhs * rhs;
}

big_integer &big_integer::operator&=(big_integer const &rhs) {
    std::function<void(word_t &, word_t)> f = [](word_t &a, word_t b) { a &= b; };
    apply_logic(rhs, f);
    this->sign &= rhs.sign;
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &rhs) {
    std::function<void(word_t &, word_t)> f = [](word_t &a, word_t b) { a |= b; };
    apply_logic(rhs, f);
    this->sign |= rhs.sign;
    return *this;
}

big_integer &big_integer::operator^=(big_integer const &rhs) {
    std::function<void(word_t &, word_t)> f = [](word_t &a, word_t b) { a ^= b; };
    apply_logic(rhs, f);
    this->sign ^= rhs.sign;
    return *this;
}

big_integer &big_integer::operator<<=(int rhs) {
    size_t big_shift = rhs / SIZEOF_WORD_T;
    size_t small_shift = rhs % SIZEOF_WORD_T;
    size_t inv_small_shift = SIZEOF_WORD_T - small_shift;

    reserve(size() + big_shift);

    if (small_shift == 0) {
        std::rotate(digits.list.begin(), digits.list.end() - 1, digits.list.end());
        return *this;
    }

    for (size_t i = size(); i > big_shift + 1; --i) {
        word_t low_bits = at(i - big_shift - 2) >> inv_small_shift;
        word_t high_bits = at(i - big_shift - 1) << small_shift;
        digits[i - 1] = low_bits | high_bits;
    }

    digits[big_shift] = at(0) << small_shift;

    fill(digits.list.begin(), digits.list.begin() + big_shift, 0);

    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    size_t big_shift = rhs / SIZEOF_WORD_T;
    size_t small_shift = rhs % SIZEOF_WORD_T;
    size_t inv_small_shift = SIZEOF_WORD_T - small_shift;

    if (small_shift == 0) {
        digits.list.erase(digits.list.begin(), digits.list.begin() + rhs);
        return *this;
    }

    for (size_t i = 0; i + big_shift < size(); ++i) {
        word_t low_bits = at(i + big_shift) >> small_shift;
        word_t high_bits = at(i + big_shift + 1) << inv_small_shift;
        digits[i] = low_bits | high_bits;
    }

    digits.resize(size() - big_shift);

    pop_leading_zeros();

    return *this;
}

big_integer big_integer::operator+() const {
    return *this;
}

big_integer big_integer::operator-() const {
    big_integer result(*this);
    result.change_sign();
    return result;
}

big_integer big_integer::operator~() const {
    big_integer result = *this;
    for (size_t i = 0; i < size(); ++i) {
        result.digits[i] = ~result.digits[i];
    }
    result.sign ^= true;
    return result;
}

big_integer &big_integer::operator++() {
    //return *this += 1;
    for (size_t i = 0; i < size(); ++i) {
        if (digits.list[i] != ZERO_MINUS) {
            ++digits.list[i];
            return *this;
        }
        digits.list[i] = 0;
    }

    digits.push_back(zero() + 1);

    bool new_sign = zero() == ZERO_MINUS ? PLUS : MINUS;
    if (new_sign != sign) {
        sign = new_sign;
    }

    pop_leading_zeros();

    return *this;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++(*this);
    return r;
}

big_integer &big_integer::operator--() {
    for (size_t i = 0; i < size(); ++i) {
        if (digits.list[i] != 0) {
            --digits.list[i];
            return *this;
        }
        digits.list[i] = ~0;
    }

    digits.push_back(zero() - 1);

    bool new_sign = zero() == ZERO_MINUS ? PLUS : MINUS;
    if (new_sign != sign) {
        sign = new_sign;
    }

    pop_leading_zeros();

    return *this;
}

big_integer big_integer::operator--(int) {
    big_integer r = *this;
    --(*this);
    return r;
}

big_integer operator+(big_integer a, big_integer const &b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const &b) {
    return a -= b;
}

big_integer operator*(big_integer a, big_integer const &b) {
    return a *= b;
}

big_integer operator/(big_integer a, big_integer const &b) {
    return a /= b;
}

big_integer operator%(big_integer a, big_integer const &b) {
    return a %= b;
}

big_integer operator&(big_integer a, big_integer const &b) {
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const &b) {
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const &b) {
    return a ^= b;
}

big_integer operator<<(big_integer a, int b) {
    return a <<= b;
}

big_integer operator>>(big_integer a, int b) {
    return a >>= b;
}

bool operator==(big_integer const &a, big_integer const &b) {
    return a.sign == b.sign && a.digits.list == b.digits.list;
}

bool operator!=(big_integer const &a, big_integer const &b) {
    return !(a == b);
}

bool operator<(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign == MINUS;
    }

    if (a.size() != b.size()) {
        return a.size() < b.size();
    }

    return std::lexicographical_compare(a.digits.list.rbegin(), a.digits.list.rend(),
                                        b.digits.list.rbegin(), b.digits.list.rend());
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign == PLUS;
    }

    if (a.size() != b.size()) {
        return a.size() > b.size();
    }

    return std::lexicographical_compare(b.digits.list.rbegin(), b.digits.list.rend(),
                                        a.digits.list.rbegin(), a.digits.list.rend());
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

std::string to_string(big_integer const &a) {
    std::string result;
    big_integer t = a;

    bool is_negative = (a.sign == MINUS);
    if (is_negative) {
        t.change_sign();
    }

    while (t > 0) {
        big_integer mod = t % BLOCK;

        std::string part = std::to_string(mod.digits[0]);
        std::reverse(part.begin(), part.end());
        part.resize(BLOCK_SIZE, '0');
        result += part;

        t /= BLOCK;
    }

    while (!result.empty() && result.back() == '0') {
        result.pop_back();
    }

    if (result.empty()) {
        result = "0";
    }

    if (is_negative) {
        result += '-';
    }
    reverse(result.begin(), result.end());
    return result;
    //return std::string(result.rbegin(), result.rend());
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

void big_integer::pop_leading_zeros() {
    while (size() > 1 && digits.back() == zero()) {
        digits.pop_back();
    }
}

word_t big_integer::at(size_t i) const {
    if (i < size()) {
        return digits[i];
    }
    return zero();
}

size_t big_integer::size() const {
    return digits.size();
}

word_t big_integer::zero() const {
    return (sign == PLUS ? ZERO_PLUS : ZERO_MINUS);
}


big_integer abs(big_integer const &a) {
    return a.sign == MINUS ? -a : a;
}

void big_integer::align(big_integer const &other) {
    reserve(other.size());
}

void big_integer::apply_logic(big_integer const &rhs, std::function<void(word_t &, word_t)> fun) {
    align(rhs);

    for (size_t i = 0; i < size(); ++i) {
        fun(digits[i], rhs.at(i));
    }

    pop_leading_zeros();
}

bool big_integer::get_significant_bit(word_t word) {
    return static_cast<bool>(word >> (SIZEOF_WORD_T - 1));
}

void big_integer::reserve(size_t n) {
    size_t new_size = std::max(size(), n);

    digits.resize(new_size, zero());
}

void big_integer::change_sign() {
    sign ^= true;
    for (size_t i = 0; i < size(); ++i) {
        digits.list[i] = ~digits.list[i];
    }
    ++(*this);

    pop_leading_zeros();
}
