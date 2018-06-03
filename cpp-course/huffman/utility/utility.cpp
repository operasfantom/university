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

size_t CRITICAL_SIZE = 100000;

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

    ifs.clear();
    ifs.seekg(0, ifs.beg);

    while (read_symbol(ifs, c)) {
        auto const& addition = tree.get_code(c);
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

    auto const& path = read_extended_bit_container(ifs);
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
    bool end_of_file = false;
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
        if (text_length >= 8){
            text_length -= 8;
        } else {
            if (end_of_file){
                throw std::invalid_argument("not enough symbols in code");
            }
            end_of_file = true;
        }
    }
}

void print_signature(std::string const &message) {
    fprintf(stderr, "Error:%s\n", message.c_str());
    fprintf(stderr, "Usage: huffman-utility [OPTION] [FILE1] [FILE2]\n");
    fprintf(stderr, "-e, --encoding, encode [FILE1] and save result to [FILE2] \n");
    fprintf(stderr, "-d, --decoding, decode [FILE1] and save result to [FILE2] \n");
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
        try {
            encoding(file_in, file_out);
        } catch (std::exception &e) {
            std::cerr << e.what();
        }
        return 0;
    }
    if (option == "-d" || option == "--decoding") {
        check_signature(3, argc);
        std::string file_in = argv[1];
        std::string file_out = argv[2];
        try {
            decoding(file_in, file_out);
        } catch (std::exception &e) {
            std::cerr << e.what();
        }
        return 0;
    }
    print_signature("wrong flag: " + std::string(argv[0]));
    return 0;
}