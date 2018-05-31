#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <codecvt>
#include <sstream>
#include "huffman_tree.h"

//namespace io {
void print_string(std::ofstream &ofs, huffman_tree::string_t const &s) {
    ofs << s.length() << '\n' << s << '\n';
}

void print_char(std::ofstream &ofs, char c) {
    ofs << c;
}//TODO SFINAE
//}

std::ifstream & read_symbol(std::ifstream &ifs, huffman_tree::symbol_t &c) {
    ifs >> c;
    return ifs;
}

size_t read_number(std::ifstream &ifs){
    size_t n;
    ifs >> n;
    return n;
}

huffman_tree::string_t read_string(std::ifstream &ifs) {
    huffman_tree::string_t result;
    size_t n;
    ifs >> n;
    result.resize(n);
    for (auto &c : result) {
        ifs >> c;
    }
    return result;
}

void encoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::ifstream ifs;
    ifs.open(file_in);
    huffman_tree::symbol_t c;
    while (read_symbol(ifs, c)) {
        tree.add(c);
    }
    ifs.close();

    try {
        tree.encoding();
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    std::ofstream ofs(file_out);

    print_string(ofs, tree.get_path());
    print_string(ofs, tree.get_dictionary());//TODO thread?

    ifs.open(file_in);
    while (ifs >> c) {
        print_string(ofs, tree.get_code(c));//TODO rvalue
    }

    ifs.close();
    ofs.close();
}

void decoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::ifstream ifs(file_in);
//    ifs >> std::noskipws;

    huffman_tree::string_t path = read_string(ifs);
    tree.set_path(path);

    huffman_tree::string_t dictionary = read_string(ifs);

    tree.set_dictionary(dictionary);//TODO move

    try {
        tree.decoding();
    } catch (std::exception &e) {
        std::cerr << e.what();
    }

    std::ofstream ofs(file_out);
    huffman_tree::symbol_t c;
    while (read_symbol(ifs, c)){
        try {
            auto p = tree.transition(c);
            if (p.second) {
                print_char(ofs, p.first);
            }
        } catch (std::exception &e) {
            std::cerr << e.what();
        }
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