#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "library.h"

void encoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::wifstream ifs(file_in);
    ifs >> std::noskipws;
    wchar_t c;
    while (ifs >> c) {
        tree.add(c);
    }
    ifs.close();

    tree.encoding();

    std::wofstream ofs(file_out);
    ofs << tree.get_path() << '\n' << tree.get_dictionary().length() << '\n' << tree.get_dictionary() << '\n';

    ifs.open(file_in);
    while (ifs >> c) {
        ofs << tree.get_code(c);
    }

    ifs.close();
    ofs.close();
}

void decoding(std::string const &file_in, std::string const &file_out) {
    huffman_tree tree;

    std::wifstream ifs(file_in);
    ifs >> std::noskipws;

    std::wstring path;
    getline(ifs, path);
    tree.set_path(path);

    size_t dict_size;
//    ifs >> std::skipws >> dict_size >> std::noskipws;
    wchar_t t;
    ifs >> dict_size >> t;

    std::wstring dictionary;
    dictionary.resize(dict_size);
    ifs.read(&dictionary[0], dict_size);
    tree.set_dictionary(dictionary);

    tree.decoding();

    std::wofstream ofs(file_out);
    wchar_t c;
    while (ifs >> c){
        auto p = tree.transition(c);
        if (p.second){
            ofs << p.first;
        }
    }
}

int main(int argc, char **argv) {
    switch (argc) {
        case 4: {
            std::string option = argv[1];
            std::string file_in = argv[2];
            std::string file_out = argv[3];
            if (option == "-e") {
                encoding(file_in, file_out);
            }
            if (option == "-d") {
                decoding(file_in, file_out);
            }
            break;
        }
        default: {
            fprintf(stderr, "Error:wrong number of arguments\n");
            fprintf(stderr, "Usage: huffman-utility [OPTION] [FILE1] [FILE2]\n");
            fprintf(stderr, "-e, --encoding, \n");
            fprintf(stderr, "-d, --decoding, \n");
        }
    }
    return 0;
}