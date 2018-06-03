//
// Created by operasfantom on 31.05.18.
//

#include "binary_io.h"


size_t read_size_t(std::ifstream &ifs) {
    size_t result;
    ifs.read(reinterpret_cast<char *>(&result), sizeof(result));
    return result;
}

huffman_tree::string_t read_extended_string(std::ifstream &ifs) {
    huffman_tree::string_t result;
    size_t n = read_size_t(ifs);
    result.resize(n);
    ifs.read(reinterpret_cast<char *>(&result[0]), n);
    return result;

}

huffman_tree::container read_extended_bit_container(std::ifstream &ifs) {
    size_t n = read_size_t(ifs);
    huffman_tree::container result(n);
    ifs.read(reinterpret_cast<char *>(&result[0]), result.char_blocks_count());
    return result;
}

void print(std::ofstream &ofs, size_t n) {
    ofs.write(reinterpret_cast<const char *>(&n), sizeof(n));
}

void print(std::ofstream &ofs, huffman_tree::string_t const &s) {
    ofs.write(reinterpret_cast<const char *>(s.data()), s.length() * sizeof(s[0]));
}

void print(std::ofstream &ofs, huffman_tree::container const &s) {
    ofs.write(reinterpret_cast<const char *>(s.data()), s.char_blocks_count());
}

void print_extended(std::ofstream &ofs, huffman_tree::string_t const &s) {
    print(ofs, s.size());
    print(ofs, s);
}

void print_extended(std::ofstream &ofs, huffman_tree::container const &s) {
    print(ofs, s.size());
    print(ofs, s);
}

