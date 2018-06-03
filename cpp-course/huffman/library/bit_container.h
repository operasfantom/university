//
// Created by operasfantom on 01.06.18.
//

#ifndef HUFFMAN_BIT_CONTAINER_H
#define HUFFMAN_BIT_CONTAINER_H

#include <vector>

template<typename W>
class bit_container : public std::vector<W> {
    static const size_t BLOCK_SIZE = 8;
    size_t sz = 0;

public:
    class bool_iterator {
        size_t position = 0;
        bit_container &container;
    public:
        bool_iterator &operator++() {
            ++position;
            return *this;
        }

        explicit bool_iterator(bit_container &container2) : container(container2) {}

        bool is_end() {
            return position >= container.size();
        }

        bool get() {
            return container.get_bit(position);
        }
    };

    bool exists_last_block()const;

    explicit bit_container(size_t n);

    bit_container();

private:
    bool extract_bit(size_t block, size_t pos) const;


    /*inline */size_t get_number_of_block(size_t i) const;

    /*inline */size_t get_position_in_block(size_t i) const;

public:
    void set_bit(size_t i, bool value);

    bool get_bit(size_t i) const;

    W get_block(size_t i) const;

    void push_back(bool x);

    void pop_back();

    void drop();

    bit_container &operator+=(bit_container const &other);

    size_t blocks_count()const;

    size_t size() const;

//    void resize(size_t);

    std::string to_string() {
        std::string result;
        for (size_t i = 0; i < sz; ++i) {
            result += (get_bit(i) ? '1' : '0');
        }
        return result;
    }
};

#include "bit_container.ipp"

#endif //HUFFMAN_BIT_CONTAINER_H
