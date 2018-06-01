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
    for (auto &c : result) {
        ifs.read(reinterpret_cast<char *>(&c), sizeof(c));
    }
    return result;

}

bit_container read_extended_bit_container(std::ifstream &ifs) {
    size_t n;
    ifs.read(reinterpret_cast<char *>(&n), sizeof(n));
    bit_container result(n);
    for (size_t i = 0; i < result.blocks_count(); ++i) {
        ifs.read(&result.at(i), sizeof(result[0]));
    }
    return result;
}

void print(std::ofstream &ofs, size_t n) {
    ofs.write(reinterpret_cast<const char *>(&n), sizeof(n));
}

void print(std::ofstream &ofs, huffman_tree::string_t const &s) {
    ofs.write(reinterpret_cast<const char *>(s.data()), s.length() * sizeof(s[0]));
}

void print(std::ofstream &ofs, bit_container const &s) {
    for (size_t i = 0; i < s.blocks_count(); ++i) {
        print(ofs, s.get_block(i));
    }
}

void print_extended(std::ofstream &ofs, huffman_tree::string_t const &s) {
    print(ofs, s.size());
    print(ofs, s);
}

void print_extended(std::ofstream &ofs, bit_container const &s) {
    print(ofs, s.size());
    print(ofs, s);
}
