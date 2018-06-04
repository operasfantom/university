//
// Created by operasfantom on 31.05.18.
//

#ifndef HUFFMAN_BINARY_IO_H
#define HUFFMAN_BINARY_IO_H

#include "huffman_tree.h"
#include <fstream>
#include <iostream>

template<typename Ch>
std::ifstream &read_symbol(std::ifstream &ifs, Ch &c) {
    ifs.read(reinterpret_cast<char *>(&c), sizeof(c));
    return ifs;
}

size_t read_size_t(std::ifstream &ifs);

huffman_tree::string_t read_extended_string(std::ifstream &ifs);

huffman_tree::container read_extended_bit_container(std::ifstream &ifs);

template <typename Ch>
void print(std::ofstream &ofs, Ch c) {
    ofs.write(reinterpret_cast<const char *>(&c), sizeof(c));
}

void print(std::ofstream &ofs, size_t n);

void print(std::ofstream &ofs, huffman_tree::string_t const &s);

void print(std::ofstream &ofs, huffman_tree::container const &s);

void print_extended(std::ofstream &ofs, huffman_tree::string_t const &s);

void print_extended(std::ofstream &ofs, huffman_tree::container const &s);

void print(std::ofstream &ofs, char *first, char *last);

#endif //HUFFMAN_BINARY_IO_H
