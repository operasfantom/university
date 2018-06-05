#ifndef HUFFMAN_BIT_CONTAINER_H
#define HUFFMAN_BIT_CONTAINER_H

#include <vector>
#include <limits>

template<typename W>
class bit_container : public std::vector<W> {
    size_t BLOCK_SIZE;
    size_t BLOCK_MASK;
    size_t BLOCK_LOG2;
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

        explicit bool_iterator(bit_container &other) : container(other) {}

        bool is_end() {
            return position >= container.size();
        }

        bool get() {
            return container.get_bit(position);
        }
    };

    explicit bit_container(size_t n);


    template<typename = typename std::enable_if<std::is_unsigned<W>::value>::type>
    bit_container() : BLOCK_SIZE(static_cast<size_t>(std::numeric_limits<W>::digits)),
                      BLOCK_MASK(BLOCK_SIZE - 1),
                      BLOCK_LOG2(__builtin_ctz(BLOCK_SIZE)) {}

private:
    void set_bit(size_t i, bool value);


    bool extract_bit(size_t block, size_t pos) const;

    /*inline */size_t get_number_of_block(size_t i) const;

    /*inline */size_t get_position_in_block(size_t i) const;
public:

    bool get_bit(size_t i) const;

    W get_block(size_t i) const;

    void pop_back();

    void push_back(bool x);

    size_t size() const;

    size_t char_blocks_count() const;

    bit_container<W> &operator+=(bit_container<W> const &other);

    /*std::string to_string() {
        std::string result;
        for (size_t i = 0; i < sz; ++i) {
            result += (get_bit(i) ? '1' : '0');
        }
        return result;
    }*/
    bit_container<W> pop();

    void wide_push_back(bool x);
};

#include "bit_container.ipp"

#endif //HUFFMAN_BIT_CONTAINER_H
