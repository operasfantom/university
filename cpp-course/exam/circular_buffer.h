#ifndef CIRCULAR_BUFFER_LIBRARY_H
#define CIRCULAR_BUFFER_LIBRARY_H

#include <iterator>
#include <memory>
#include <algorithm>
#include <new>
#include <cassert>

constexpr size_t operator "" _sz(unsigned long long n) {
    return n;
}

template<typename T>
class circular_buffer {
private:
    size_t sz = 0, cap = 0;
    T *data_ = nullptr, *begin_pos = nullptr, *end_pos = nullptr;

    inline static size_t save_mod(size_t x, size_t m){
        return (x >= m ? x - m : x);
    }

    void construct(T *p, T const &value) {
        new(p) T(value);
    }

    void pointer_inc(T *&p) const {
        ++p;
        if (p == data_ + cap) {
            p = data_;
        }
    }

    void pointer_dec(T *&p) const {
        if (p == data_) {
            p = data_ + cap - 1;
        } else {
            --p;
        }
    }

    template<typename U = T>
    static typename std::enable_if<std::is_trivially_destructible<U>::value>::type destruct(U &p) {}

    template<typename U = T>
    static typename std::enable_if<not std::is_trivially_destructible<U>::value>::type destruct(U &p) {
        p.~U();
    }

    bool need_expand() {
        return (sz + 1 >= cap);
    }

    void change_capacity() {
        change_capacity(std::max(2_sz, cap * 2));
    }

    void change_capacity(size_t n) { //n >= cap
        if (n == 0) return;
        T *new_data = static_cast<T *>(operator new(n * sizeof(T)));
        size_t i = 0;
        circular_buffer tmp;
        try {
            for (auto it = begin(); it != end(); ++it, ++i) {
                construct(new_data + i, *it);
            }
            for (auto it = begin(); it != end(); ++it) {
                destruct(*it.vertex);
            }
            operator delete(data_);
            data_ = new_data;
        } catch (...) {
            for (size_t j = 0; j != i; ++j) {
                destruct(new_data[j]);
            }
            operator delete(new_data);
            throw;
        }

        cap = n;
        begin_pos = data_;
        end_pos = data_ + sz;
    }
    //endregion
public:
    template<bool is_const = true>
    class any_cv_iterator :
            public std::iterator<
                    std::bidirectional_iterator_tag,
                    T,
                    std::ptrdiff_t,
                    typename std::conditional<is_const, T const *, T *>::type,
                    typename std::conditional<is_const, T const &, T &>::type
            > {
        typedef std::iterator<
                std::bidirectional_iterator_tag,
                T,
                std::ptrdiff_t,
                typename std::conditional<is_const, T const *, T *>::type,
                typename std::conditional<is_const, T const &, T &>::type
        > base_t;

        circular_buffer<T> const *buf;
        T *vertex;

        friend circular_buffer<T>;

        T *get() {
            return vertex;
        }

    public:
        any_cv_iterator() noexcept = default;

        any_cv_iterator(circular_buffer<T> const *container, T *p) noexcept : buf(container), vertex(p) {};

        any_cv_iterator(any_cv_iterator<false> const &other) noexcept : buf(other.buf), vertex(other.vertex) {}

        any_cv_iterator &operator=(any_cv_iterator const &other) noexcept {
            if (this == &other) {
                return *this;
            }
            any_cv_iterator it(other);
            swap(*this, it);
            return *this;
        }

        virtual ~any_cv_iterator() = default;

        any_cv_iterator &operator++() {
            buf->pointer_inc(vertex);
            return *this;
        }

        any_cv_iterator operator++(int) {
            auto it = *this;
            ++*this;
            return it;
        }

        any_cv_iterator &operator--() {
            buf->pointer_dec(vertex);
            return *this;
        }

        any_cv_iterator operator--(int) {
            auto it = *this;
            --*this;
            return it;
        }

        any_cv_iterator &operator+=(typename base_t::difference_type delta) {
            vertex = buf->begin_pos + save_mod(vertex - buf->begin_pos + delta, buf->cap);
            return *this;
        }

        any_cv_iterator &operator-=(typename base_t::difference_type delta) {
            vertex = buf->begin_pos + save_mod(vertex - buf->begin_pos + buf->cap - delta, buf->cap);;
            return *this;
        }

        any_cv_iterator operator+(typename base_t::difference_type delta) const {
            auto it = *this;
            return it += delta;
        }

        any_cv_iterator operator-(typename base_t::difference_type delta) const {
            auto it = *this;
            return it -= delta;
        }

        typename base_t::difference_type operator-(any_cv_iterator const &other) const {
            assert(buf == other.buf);
            return save_mod(vertex + buf->cap - other.vertex, buf->cap);
        }

        size_t get_pos() const noexcept {
            return *this - buf->begin();
        }

        template<bool any_cv>
        bool operator<(any_cv_iterator<any_cv> const &other) const noexcept {
            return this->get_pos() < other.get_pos();
        }

        template<bool any_cv>
        bool operator>(any_cv_iterator<any_cv> const &other) const noexcept {
            return this->get_pos() > other.get_pos();
        }

        template<bool any_cv>
        bool operator==(any_cv_iterator<any_cv> const &other) const noexcept {
            return this->vertex == other.vertex;
        }

        template<bool any_cv>
        bool operator!=(any_cv_iterator<any_cv> const &other) const noexcept {
            return !(*this == other);
        }

        template<bool any_cv>
        bool operator<=(any_cv_iterator<any_cv> const &other) const noexcept {
            return (this->get_pos() < other.get_pos()) || (*this == other);
        }

        template<bool any_cv>
        bool operator>=(any_cv_iterator<any_cv> const &other) const noexcept {
            return (this->get_pos() > other.get_pos()) || (*this == other);
        }

        typename base_t::reference operator*() const noexcept {
            return *vertex;
        }

        typename base_t::pointer operator->() const noexcept {
            return vertex;
        }

        friend void swap(any_cv_iterator &a, any_cv_iterator &b) noexcept {
            std::swap(a.vertex, b.vertex);
            std::swap(a.buf, b.buf);
        };
    };

public:
    //region Typedefs

    typedef any_cv_iterator<false> iterator;
    typedef any_cv_iterator<true> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    /*typedef cb_reverse_iterator<false> reverse_iterator;
    typedef cb_reverse_iterator<true> const_reverse_iterator;*/
    //endregion

    //region Constructors

    circular_buffer() noexcept = default;

    circular_buffer(circular_buffer const &other) : circular_buffer() {
        change_capacity(other.sz);
        for (auto it = other.begin(); it != other.end(); ++it) {
            push_back(*it);
        }
    }

    circular_buffer &operator=(circular_buffer const &other) {
        if (this == &other) {
            return *this;
        }
        circular_buffer tmp(other);
        swap(*this, tmp);
        return *this;
    }

    virtual ~circular_buffer() {
        for (auto it = begin(); it != end(); ++it) {
            destruct(*it);
        }
        operator delete(data_);
    };
    //endregion

    //region Interface

    T &front();

    T const &front() const;

    T &back();

    T const &back() const;

    void push_front(T const &value);

    void push_back(T const &value);

    void pop_front();

    void pop_back();

    T &operator[](size_t index);

    T const &operator[](size_t index) const;

    bool empty() const noexcept;

    size_t size();

    void clear() noexcept;

    iterator insert(const_iterator, T const &);

    iterator erase(const_iterator);

    template<typename S>
    friend void swap(circular_buffer<S> &, circular_buffer<S> &) noexcept;
    //endregion

    //region Begin-End

    iterator begin() { return iterator(this, begin_pos); }

    const_iterator begin() const { return const_iterator(this, begin_pos); }

    iterator end() { return iterator(this, end_pos); }

    const_iterator end() const { return const_iterator(this, end_pos); }

    reverse_iterator rbegin() { return reverse_iterator(end()); }

    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }

    reverse_iterator rend() { return reverse_iterator(begin()); }

    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    //endregion

private:
    bool closer_to_the_beginning(const_iterator it) const {
        return it.get_pos() < it.buf->sz / 2;
    }

    void shift_back(iterator it1, iterator it2) {
        for (auto itr = it1, itl = --it1; itl != it2; ++itl, ++itr) {
            std::swap(*itl, *itr);
        }
    }

    void shift_forward(iterator it1, iterator it2) {
        for (auto itl = it2, itr = ++it2; itr != it1; --itl, --itr) {
            std::swap(*itl, *itr);
        }
    }
};

template<typename T>
T &circular_buffer<T>::front() {
    return *begin();
}

template<typename T>
T const &circular_buffer<T>::front() const {
    return *begin();
}

template<typename T>
T &circular_buffer<T>::back() {
    return *rbegin();
}

template<typename T>
T const &circular_buffer<T>::back() const {
    return *rbegin();
}

template<typename T>
void circular_buffer<T>::push_front(const T &value) {
    if (need_expand()) {
        change_capacity();
    }
    pointer_dec(begin_pos);
    try{
        construct(begin_pos, value);
    } catch (...) {
        pointer_inc(begin_pos);
        throw;
    }
    ++sz;
}

template<typename T>
void circular_buffer<T>::push_back(const T &value) {
    if (need_expand()) {
        change_capacity();
    }
    construct(end_pos, value);
    pointer_inc(end_pos);
    ++sz;
}

template<typename T>
void circular_buffer<T>::pop_front() {
    destruct(*begin_pos);
    pointer_inc(begin_pos);
    --sz;
}

template<typename T>
void circular_buffer<T>::pop_back() {
    pointer_dec(end_pos);
    destruct(*end_pos);
    --sz;
}

template<typename T>
T &circular_buffer<T>::operator[](size_t index) {
    assert(index < sz);
    return data_[((begin_pos - data_) + index) % cap];
}

template<typename T>
T const &circular_buffer<T>::operator[](size_t index) const {
    assert(index < sz);
    return data_[((begin_pos - data_) + index) % cap];
}

template<typename T>
bool circular_buffer<T>::empty() const noexcept {
    return begin() == end();
}

template<typename T>
size_t circular_buffer<T>::size() {
    return sz;
}

template<typename T>
void circular_buffer<T>::clear() noexcept {
    while (begin() != end()) {
        pop_front();
    }
    sz = 0;
}

template<typename T>
typename circular_buffer<T>::iterator circular_buffer<T>::insert(circular_buffer::const_iterator pos, const T &value) {
    bool from_front = closer_to_the_beginning(pos);
    size_t index = pos.get_pos();
    if (need_expand()) {
        change_capacity();
    }
    iterator non_const_pos = begin() + index;
    iterator result = non_const_pos;
    if (from_front) {
        construct((begin() - 1).vertex, value);
        shift_back(begin(), non_const_pos - 1);
        pointer_dec(begin_pos);
        --result;
    } else {
        construct(end().vertex, value);
        shift_forward(non_const_pos, end() - 1);
        pointer_inc(end_pos);
    }
    ++sz;
    return result;
}

template<typename T>
typename circular_buffer<T>::iterator circular_buffer<T>::erase(circular_buffer::const_iterator pos) {
    bool from_front = closer_to_the_beginning(pos);
    iterator non_const_pos(this, pos.get());
    iterator result = non_const_pos;
    if (from_front) {
        shift_forward(begin(), non_const_pos - 1);
        destruct(*begin_pos);
        pointer_inc(begin_pos);
        ++result;
    } else {
        shift_back(non_const_pos + 1, end() - 1);
        pointer_dec(end_pos);
        destruct(*end_pos);
    }
    --sz;
    return result;
}

template<typename S>
void swap(circular_buffer<S> &a, circular_buffer<S> &b) noexcept {
    std::swap(a.sz, b.sz);
    std::swap(a.cap, b.cap);
    std::swap(a.data_, b.data_);
    std::swap(a.begin_pos, b.begin_pos);
    std::swap(a.end_pos, b.end_pos);
}


#endif
