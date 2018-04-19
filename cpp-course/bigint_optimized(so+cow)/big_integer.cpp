#include "big_integer.h"
#include "arithmetic.h"

#include <algorithm>
#include <deque>

big_integer::big_integer() : sign(PLUS), type(SMALL), number(0) {}

big_integer::big_integer(big_integer const &other) : sign(other.sign), type(other.type) {
    if (other.is_small_object()) {
        number = other.number;
    } else {
        //list.~dynamic_storage();
        list = other.list;
    }
}

big_integer::~big_integer() {
    if (is_big_object()) {
        list.~dynamic_storage();
    }
}

big_integer::big_integer(int a) : sign(a >= 0 ? PLUS : MINUS), type(SMALL), number(a) {}

big_integer &big_integer::operator=(big_integer const &other) {
    if (type == other.type) {
        if (is_small_object()) { // S S
            number = other.number;
        } else { // B B
            list.~dynamic_storage();
            list = other.list;
        }
    } else {
        if (is_small_object()) { // S B
            list = other.list;
        } else { // B S
            list.~dynamic_storage();
            number = other.number;
        }
    }
    sign = other.sign;
    type = other.type;
    return *this;
}

big_integer::big_integer(word_t a) : sign(PLUS), type(SMALL), number(a) {}

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

big_integer &big_integer::operator+=(big_integer const &rhs) {
    align(rhs);

    word_t cf = 0;
    for (size_t i = 0; i < size(); ++i) {
        (*this)[i] = simple_add(at(i), rhs.at(i), cf);
    }

    if (cf != 0) {
        if (this->sign != rhs.sign) {
            sign = PLUS;
        } else {
            if (this->sign == MINUS) {
                sign = MINUS;
            } else {
                push_back(1);
                sign = PLUS;
            }
        }
    } else {
        sign ^= rhs.sign;
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
        word_t rdx = 0, cf = 0, factor = absolute_a[i];
        for (size_t j = 0; j < absolute_b.size(); ++j) {
            result[i + j] = simple_add(result[i + j], rdx, cf);
            word_t rax = simple_mul(factor, absolute_b[j], rdx);
            rdx += cf;
            cf = 0;
            result[i + j] = simple_add(result[i + j], rax, cf);
        }
        result[i + absolute_b.size()] = rdx + cf;
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
    if (absolute_b.back() < WORD_MAX / 2) {
        scale = WORD_MAX / (absolute_b.back() + 1);
        absolute_a *= scale;
        absolute_b *= scale;
    }


    big_integer dividend;

    for (size_t i = absolute_a.size(); i > 0; --i) { // dividend / divider = quotient
        big_integer &divider = absolute_b;

        dividend <<= SIZEOF_WORD_T;
        dividend[0] = absolute_a[i - 1];

        if (dividend.size() < divider.size()) {
            result.push_back(0);
            continue;
        }

        if (dividend.size() == divider.size()) {
            dividend.push_back(0);
        }

        __uint128_t guess = dividend.back();

        guess <<= SIZEOF_WORD_T;
        guess |= dividend[dividend.size() - 2];

        guess /= static_cast<__uint128_t>(divider.back());

        big_integer composition;
        composition.reserve(2);
        composition[0] = static_cast<word_t>(guess);
        composition[1] = static_cast<word_t>(guess >> SIZEOF_WORD_T);

        composition *= divider;

        //size_t steps = 0;
        while (guess > 0 && composition > dividend) {
            --guess;
            composition -= divider;

            //steps++;
        }

        //assert(steps <= 2);

        result.push_back(static_cast<size_t>(guess));

        //composition.pop_leading_zeros();

        dividend -= composition;
    }

    result.reverse();

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
        std::rotate(list.begin(), list.end() - 1, list.end());
        return *this;
    }

    for (size_t i = size(); i > big_shift + 1; --i) {
        word_t low_bits = at(i - big_shift - 2) >> inv_small_shift;
        word_t high_bits = at(i - big_shift - 1) << small_shift;
        (*this)[i - 1] = low_bits | high_bits;
    }

    (*this)[big_shift] = at(0) << small_shift;

    std::fill(list.begin(), list.begin() + big_shift, 0);

    pop_leading_zeros();

    return *this;
}

big_integer &big_integer::operator>>=(int rhs) {
    size_t big_shift = rhs / SIZEOF_WORD_T;
    size_t small_shift = rhs % SIZEOF_WORD_T;
    size_t inv_small_shift = SIZEOF_WORD_T - small_shift;

    if (small_shift == 0) {
        list.erase(0, rhs);
        return *this;
    }

    for (size_t i = 0; i + big_shift < size(); ++i) {
        word_t low_bits = at(i + big_shift) >> small_shift;
        word_t high_bits = at(i + big_shift + 1) << inv_small_shift;
        (*this)[i] = low_bits | high_bits;
    }

    for (size_t i = 0; i < big_shift; ++i) {
        pop_back();
    }

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
    if (is_small_object()) {
        big_integer result(*this);
        result.number = ~result.number;
        result.sign ^= true;
        return result;
    }

    big_integer result = *this;
    for (size_t i = 0; i < size(); ++i) {
        result[i] = ~result[i];
    }
    result.sign ^= true;
    return result;
}

big_integer &big_integer::operator++() {
    return *this += 1;
}

big_integer big_integer::operator++(int) {
    big_integer r = *this;
    ++(*this);
    return r;
}

big_integer &big_integer::operator--() {
    return *this -= 1;
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
    if (a.sign != b.sign) {
        return false;
    }
    if (a.is_small_object() && b.is_small_object()) {
        return a.number == b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list == b.list;
    } else {
        return a.is_small_object();
    }
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

    if (a.is_small_object() && b.is_small_object()) {
        return a.number < b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list < b.list;
    } else {
        return a.is_small_object();
    }
}

bool operator>(big_integer const &a, big_integer const &b) {
    if (a.sign != b.sign) {
        return a.sign == PLUS;
    }

    if (a.sign != b.sign) {
        return a.sign == MINUS;
    }

    if (a.size() != b.size()) {
        return a.size() > b.size();
    }

    if (a.is_small_object() && b.is_small_object()) {
        return a.number > b.number;
    } else if (a.is_big_object() && a.is_big_object()) {
        return a.list > b.list;
    } else {
        return a.is_big_object();
    }
}

bool operator<=(big_integer const &a, big_integer const &b) {
    return !(a > b);
}

bool operator>=(big_integer const &a, big_integer const &b) {
    return !(a < b);
}

std::string to_string(big_integer const &a) {
    if (a.is_small_object()) {
        if (a.sign == PLUS) {
            return std::to_string(a.number);
        } else {
            return "-" + std::to_string(~a.number + 1);
        }
    }


    std::string result;
    big_integer t = a;


    bool is_negative = (a.sign == MINUS);
    if (is_negative) {
        t.change_sign();
    }

    while (t > 0) {
        big_integer mod = t % BLOCK;

        std::string part = std::to_string(mod[0]);
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
    if (is_small_object()) {
        return;
    }

    while (size() > 1 && back() == zero()) {
        pop_back();
    }

    if (size() == 1) {
        transform_to_small_object();
    }
}

word_t big_integer::at(size_t i) const {
    if (i < size()) {
        return (*this)[i];
    }
    return zero();
}

size_t big_integer::size() const {
    if (is_small_object()) {
        return 1;
    }
    return list.size;
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

    if (is_small_object() && rhs.is_small_object()) {
        fun(number, rhs.number);
        return;
    }

    for (size_t i = 0; i < size(); ++i) {
        fun((*this)[i], rhs.at(i));
    }

    pop_leading_zeros();
}

bool big_integer::get_significant_bit(word_t word) {
    return static_cast<bool>(word >> (SIZEOF_WORD_T - 1));
}

void big_integer::reserve(size_t n) {
    if (n <= size()) {
        return;
    }
    if (is_small_object()) {
        transform_to_big_object();
    }
    size_t new_size = std::max(size(), n);

    reserve(new_size, zero());
}

void big_integer::change_sign() {
    if (is_small_object()) {
        number = ~number + 1;
        if (number != 0) {
            sign ^= true;
        }
        return;
    }

    sign ^= true;
    for (size_t i = 0; i < size(); ++i) {
        (list)[i] = ~(list)[i];
    }
    ++(*this);

    pop_leading_zeros();
}

void big_integer::transform_to_big_object() {
    change_type();
}

void big_integer::transform_to_small_object() {
    change_type();
}

bool big_integer::is_small_object() const {
    return type == SMALL;
}

bool big_integer::is_big_object() const {
    return type == BIG;
}

word_t &big_integer::operator[](size_t i) {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        return list[i];
    }
}

word_t big_integer::operator[](size_t i) const {
    if (is_small_object()) {
        assert(i == 0);
        return number;
    } else {
        return list[i];
    }
}

void big_integer::push_back(size_t val) {
    if (is_small_object()) {
        change_type();
    }
    list.push_back(val);
}

void big_integer::pop_back() {
    list.pop_back();
}

word_t big_integer::back() const {
    if (is_small_object()) {
        return number;
    } else {
        return list.back();
    }
}

void big_integer::reverse() {
    if (is_small_object()) {
        return;
    }
    std::reverse(list.begin(), list.end());
}

void big_integer::resize(size_t n) {
    if (n > size()) {
        reserve(n, 0);
    } else {
        if (is_small_object()) {
            return;
        }
        list.resize(n);
    }
}

void big_integer::reserve(size_t n, word_t val) {
    if (is_small_object()) {
        if (n > 1) {
            change_type();
        } else {
            return;;
        }
    }
    list.reserve(n, val);
}

void big_integer::change_type() {
    if (is_small_object()) {
        list = dynamic_storage<word_t>(number);
    } else {
        word_t tmp = list[0];
        list.~dynamic_storage();
        number = tmp;
    }
    type ^= true;
}

size_t big_integer::size() {
    if (is_small_object()) {
        return 1;
    } else {
        return list.size;
    }
}
