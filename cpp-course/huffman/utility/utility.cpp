#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <codecvt>
#include <sstream>
#include <cstring>
#include <fstream>
#include "huffman_tree.h"
#include "binary_io.h"
#include "bit_container.h"

size_t CRITICAL_SIZE = 20;//TODO

using symbol_t = huffman_tree::symbol_t;
using string_t = huffman_tree::string_t;

#define min(a, b) ((a) < (b) ? (a) : (b))

void encoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::ifstream ifs(file_in, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("couldn't open input file");
    }

    symbol_t c;
    while (read_symbol(ifs, c)) {
        tree.add(c);
    }

    try {
        tree.encoding();
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    std::ofstream ofs(file_out, std::ios::binary);
    if (!ofs.is_open()) {
        throw std::runtime_error("couldn't open output file");
    }

    print_extended(ofs, tree.get_path());
    print_extended(ofs, tree.get_dictionary());//TODO move?

    bit_container acc;
    auto check_acc_size = [&ofs, &acc]() {
        if (acc.size() > CRITICAL_SIZE) {
            size_t number_of_blocks = acc.blocks_count() - (acc.exists_last_block() ? 1 : 0);
            for (size_t i = 0; i < number_of_blocks; ++i) {
                char c = acc.get_block(i);
                print(ofs, c);
            }
            acc.drop();
        }
    };

    size_t text_length = tree.get_text_length();
    print(ofs, text_length);

    ifs.close();
    ifs.open(file_in);//TODO

    while (read_symbol(ifs, c)) {
        auto addition = tree.get_code(c);
        acc += addition;
        check_acc_size();
    }
    print(ofs, acc);

    ifs.close();
    ofs.close();
}

void decoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::ifstream ifs(file_in, std::ios::binary);
    if (!ifs.is_open()) {
        throw std::runtime_error("couldn't open input file");
    }

    auto path = read_extended_bit_container(ifs);
    tree.set_path(path);
    
    string_t dictionary = read_extended_string(ifs);

    tree.set_dictionary(dictionary);//TODO move

    try {
        tree.decoding();
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    std::ofstream ofs(file_out, std::ios::binary);
    if (!ofs.is_open()) {
        throw std::runtime_error("couldn't open output file");
    }

    size_t text_length = read_size_t(ifs);
    char t;
    while (read_symbol(ifs, t)) {
        for (size_t i = 0; i < min(text_length, 8); ++i) {
            bool c = static_cast<bool>((t >> i) & 1);
            try {
                auto p = tree.transition(c);
                if (p.second) {
                    print(ofs, p.first);
                }
            } catch (std::exception &e) {
                std::cerr << e.what();
            }
        }
        text_length -= 8;
    }
}

void print_signature(std::string const &message) {
    fprintf(stderr, "Error:%s\n", message.c_str());
    fprintf(stderr, "Usage: huffman-utility [OPTION] [FILE1] [FILE2]\n");
    fprintf(stderr, "-e, --encoding, \n");
    fprintf(stderr, "-d, --decoding, \n");
}

void check_signature(int expected, int result) {
    if (expected != result) {
        print_signature("expected: " + std::to_string(expected) + "arguments, result: " + std::to_string(result));
        exit(0);
    }
}

int main(int argc, char **argv) {
    --argc;
    ++argv;
    if (argc == 0) {
        print_signature("there is no flag");
        return 0;
    }
    std::string option = argv[0];
    if (option == "-e" || option == "--encoding") {
        check_signature(3, argc);
        std::string file_in = argv[1];
        std::string file_out = argv[2];
        encoding(file_in, file_out);
        return 0;
    }
    if (option == "-d" || option == "--decoding") {
        check_signature(3, argc);
        std::string file_in = argv[1];
        std::string file_out = argv[2];
        decoding(file_in, file_out);
        return 0;
    }
    print_signature("wrong flag: " + std::string(argv[0]));
    return 0;
}