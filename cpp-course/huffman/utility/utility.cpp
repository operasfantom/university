#include <iostream>
#include <fstream>
#include <algorithm>
#include <codecvt>
#include <sstream>
#include <cstring>
#include "huffman_tree.h"
#include "binary_io.h"

using symbol_t = huffman_tree::symbol_t;
using string_t = huffman_tree::string_t;
using container = huffman_tree::container;

#define min(a, b) ((a) < (b) ? (a) : (b))

const size_t CRITICAL_SIZE = 1ull << 16;
const size_t CHAR_DIGITS = 8;

char buffer_input[CRITICAL_SIZE];
char buffer_output[CRITICAL_SIZE];

void check_acc_size(std::ostream &ofs, container &acc) {
    if (acc.char_blocks_count() > CRITICAL_SIZE) {
        if (!acc.empty()) {
            auto t = acc.pop();
            print(ofs, acc);
            acc = t;
        }
    }
}

void encoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    try{
        binary_file input(file_in, std::ios::in);
        input.set_buffer(buffer_input, sizeof(buffer_input));
        while (input.has_next_char()){
            char c = input.next_char();
            tree.add(static_cast<symbol_t>(c));
        }
        tree.encoding();

        binary_file output(file_out, std::ios::out);

        print_extended(output.get_stream(), tree.get_path());
        print_extended(output.get_stream(), tree.get_dictionary());

        size_t text_length = tree.get_text_length();
        print(output.get_stream(), text_length);

        input.reopen();
        container acc;
        while (input.has_next_char()){
            char c = input.next_char();
            auto const& addition = tree.get_code(static_cast<symbol_t>(c));
            acc += addition;
            check_acc_size(output.get_stream(), acc);
        }
        print(output.get_stream(), acc);
    } catch (std::exception const &e){
        throw;
    }
}

void decoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    try {
        binary_file input(file_in, std::ios::in);
        input.set_buffer(buffer_input, sizeof(buffer_input));
        auto const &path = read_extended_bit_container(input.get_stream());
        tree.set_path(path);
        string_t dictionary = read_extended_string(input.get_stream());
        tree.set_dictionary(dictionary);
        tree.decoding();

        binary_file output(file_out, std::ios::out);
        size_t text_length = read_size_t(input.get_stream());
        output.set_buffer(buffer_output, sizeof(buffer_output));
        while (input.has_next_char()) {
            char c = input.next_char();
            for (size_t i = 0; i < min(text_length, CHAR_DIGITS); ++i) {
                if (tree.transition(c & 1, output.get_target())){
                    output.move_forward_position();
                }
                c >>= 1;
                if (output.full()) {
                    output.dump(output.get_stream());
                }
            }
            text_length -= CHAR_DIGITS;
        }
        output.dump(output.get_stream());
    } catch (std::exception const &e) {
        throw;
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