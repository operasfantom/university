//
// Created by operasfantom on 31.05.18.
//

#ifndef HUFFMAN_BINARY_IO_H
#define HUFFMAN_BINARY_IO_H

#include "huffman_tree.h"
#include <fstream>
#include <iostream>
#include <memory>

template<typename Ch>
std::istream &read_symbol(std::istream &ifs, Ch &c) {
    ifs.read(reinterpret_cast<char *>(&c), sizeof(c));
    return ifs;
}

size_t read_size_t(std::istream &ifs);

huffman_tree::string_t read_extended_string(std::istream &ifs);

huffman_tree::container read_extended_bit_container(std::istream &ifs);

template <typename Ch>
void print(std::ostream &ofs, Ch c) {
    ofs.write(reinterpret_cast<const char *>(&c), sizeof(c));
}

void print(std::ostream &ofs, size_t n);

void print(std::ostream &ofs, huffman_tree::string_t const &s);

void print(std::ostream &ofs, huffman_tree::container const &s);

void print_extended(std::ostream &ofs, huffman_tree::string_t const &s);

void print_extended(std::ostream &ofs, huffman_tree::container const &s);

void print(std::ostream &ofs, char *first, char *last);

class binary_file {
    bool mode_in;
    std::fstream stream;

    char *buffer = nullptr, *position = nullptr, *last = nullptr;
    size_t buffer_size = 0;

    void initialize_last();
public:
    binary_file(std::string const &, std::ios_base::openmode);

    virtual ~binary_file();

    void read_buffer();

    bool has_next_char();

    char next_char();

    void set_buffer(char *, size_t);

    char * get_target();

    bool full();

    std::iostream & get_stream();

    void dump(std::ostream &);

    void move_forward_position();

    void reopen();
};
#endif //HUFFMAN_BINARY_IO_H
