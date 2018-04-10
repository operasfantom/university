#include "big_integer.h"
#include "arithmetic.h"

#include <algorithm>
#include <deque>

big_integer::big_integer() : sign(PLUS), data() {}

big_integer::big_integer(big_integer const &other) = default;

big_integer::big_integer(int a) : sign(a >= 0 ? PLUS : MINUS), data(a) {}

big_integer::big_integer(word_t a) : sign(PLUS), data(a) {}

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
        data[i] = simple_add(at(i), rhs.at(i), cf);
    }

    data.push_back(simple_add(zero(), rhs.zero(), cf));

    bool new_sign = get_significant_bit(data.back()) != 0 ? MINUS : PLUS;
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

    big_integer absolute_a = absolute(*this);
    big_integer absolute_b = absolute(rhs);
    big_integer result;

    result.reserve(absolute_a.size() + absolute_b.size());

    for (size_t i = 0; i < absolute_a.size(); ++i) {
        word_t rdx = 0, cf = 0, factor = absolute_a.data[i];
        for (size_t j = 0; j < absolute_b.size(); ++j) {
            result.data[i + j] = simple_add(result.data[i + j], rdx, cf);
            word_t rax = simple_mul(factor, absolute_b.data[j], rdx);
            rdx += cf;
            cf = 0;
            result.data[i + j] = simple_add(result.data[i + j], rax, cf);
        }
        result.data[i + absolute_b.size()] = rdx + cf;
    }

    result.pop_leading_zeros();

    if (result_sign == MINUS) {
        result.change_sign();
    }

    return *this = result;
}

big_integer &big_integer::operator/=(big_integer const &rhs) {
    bool result_sign = this->sign ^rhs.sign;

    big_integer absolute_a = absolute(*this);
    big_integer absolute_b = absolute(rhs);
    big_integer result;

    result.reserve(absolute_a.size());

    word_t scale;
    if (absolute_b.data.back() < WORD_MAX / 2) {
        scale = WORD_MAX / (absolute_b.data.back() + 1);
        absolute_a *= scale;
        absolute_b *= scale;
    }

    big_integer dividend;

    for (size_t i = absolute_a.size(); i > 0; --i) { // dividend / divider = quotient
        big_integer &divider = absolute_b;

        dividend <<= SIZEOF_WORD_T;
        dividend |= absolute_a.data[i - 1];

        dividend.pop_leading_zeros();

        if (dividend.size() < divider.size()) {
            result.data.push_back(0);
            continue;
        }

        if (dividend.size() == divider.size()) {
            dividend.data.push_back(0);
        }

        __uint128_t guess = dividend.data.back();

        guess <<= SIZEOF_WORD_T;
        guess |= dividend.data[dividend.size() - 2];

        guess /= static_cast<__uint128_t>(divider.data.back());

        big_integer composition;
        composition.reserve(2);
        composition.data[0] = static_cast<word_t>(guess);
        composition.data[1] = static_cast<word_t>(guess >> SIZEOF_WORD_T);

        composition *= divider;

        //size_t steps = 0;
        while (guess > 0 && composition > dividend) {
            --guess;
            composition -= divider;

            //steps++;
        }

        //assert(steps <= 2);

        result.data.push_back(static_cast<size_t>(guess));

        dividend -= composition;
    }

    result.data.reverse();

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
        std::rotate(data.list->begin(), data.list->end() - 1, data.list->end());
        return *this;
    }

    for (size_t i = size(); i > big_shift + 1; --i) {
        word_t low_bits = at(i - big_shift - 2) >> inv_small_shift;
        word_t high_bits = at(i - big_shift - 1) << small_shift;
        data[i - 1] = low_bits | high_bits;
    }

    data[big_shift] = at(0) << small_shift;

    std::fill(data.list->begin(), data.list->begin() + big_shift, 0);

    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    size_t big_shift = rhs / SIZEOF_WORD_T;
    size_t small_shift = rhs % SIZEOF_WORD_T;
    size_t inv_small_shift = SIZEOF_WORD_T - small_shift;

    if (small_shift == 0) {
        data.list->erase(0, rhs);
        return *this;
    }

    for (size_t i = 0; i + big_shift < size(); ++i) {
        word_t low_bits = at(i + big_shift) >> small_shift;
        word_t high_bits = at(i + big_shift + 1) << inv_small_shift;
        data[i] = low_bits | high_bits;
    }

    data.resize(size() - big_shift);

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
        result.data[i] = ~result.data[i];
    }
    result.sign ^= true;
    return result;
}

big_integer &big_integer::operator++() {
    //return *this += 1;
    for (size_t i = 0; i < size(); ++i) {
        if (data[i] != ZERO_MINUS) {
            ++data[i];
            return *this;
        }
        data[i] = ZERO_PLUS;
    }

    data.push_back(zero() + 1);

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
        if (data[i] != 0) {
            --data[i];
            return *this;
        }
        data[i] = ZERO_MINUS;
    }

    data.push_back(zero() - 1);

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
    return a.sign == b.sign && a.data == b.data;
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

    return a.data < b.data;
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign == PLUS;
    }

    if (a.size() != b.size()) {
        return a.size() > b.size();
    }

    return a.data > b.data;
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

        std::string part = std::to_string(mod.data[0]);
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
    return std::string(result.rbegin(), result.rend());
}

std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    return s << to_string(a);
}

void big_integer::pop_leading_zeros() {
    while (size() > 1 && data.back() == zero()) {
        data.pop_back();
    }
}

word_t big_integer::at(size_t i) const {
    if (i < size()) {
        return data[i];
    }
    return zero();
}

size_t big_integer::size() const {
    return data.list->size;
}

word_t big_integer::zero() const {
    return (sign == PLUS ? ZERO_PLUS : ZERO_MINUS);
}

big_integer absolute(big_integer const &a) {
    return a.sign == MINUS ? -a : a;
}

void big_integer::align(big_integer const &other) {
    reserve(other.size());
}

void big_integer::apply_logic(big_integer const &rhs, std::function<void(word_t &, word_t)> fun) {
    align(rhs);

    for (size_t i = 0; i < size(); ++i) {
        fun(data[i], rhs.at(i));
    }

    pop_leading_zeros();
}

bool big_integer::get_significant_bit(word_t word) {
    return static_cast<bool>(word >> (SIZEOF_WORD_T - 1));
}

void big_integer::reserve(size_t n) {
    size_t new_size = std::max(size(), n);

    data.resize(new_size, zero());
}

void big_integer::change_sign() {
    sign ^= true;
    for (size_t i = 0; i < size(); ++i) {
        (*data.list)[i] = ~(*data.list)[i];
    }
    ++(*this);

    pop_leading_zeros();
}
