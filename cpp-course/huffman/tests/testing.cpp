#include <cmath>
#include "huffman_tree.h"
#include "gtest/gtest.h"

void my_check(std::string input) {
    huffman_tree tree_encode;

    for (auto c : input) {
        tree_encode.add(c);
    }

    tree_encode.encoding();

    huffman_tree::string_t code;
    for (auto c : input) {
        code += tree_encode.get_code(c);
    }

    double n = input.length();
    EXPECT_TRUE(code.length() <= n * std::log2(n + 1));

    huffman_tree::string_t path = tree_encode.get_path();
    huffman_tree::string_t dictionary = tree_encode.get_dictionary();

    huffman_tree tree_decode;

    //---

    tree_decode.set_path(path);
    tree_decode.set_dictionary(dictionary);
    tree_decode.decoding();

    std::string result;
    for (auto c : code) {
        auto p = tree_decode.transition(c);
        if (p.second) {
            result += p.first;
        }
    }

    EXPECT_EQ(input, result);
}

TEST(correctness, sample) {
    my_check("abracadabra");
}

TEST(correctness, empty) {
    my_check("");
}

TEST(correctness, one) {
    my_check("1");
}

TEST(correctness, back_slash_n) {
    my_check("\n");
}

TEST(correctness, back_slash_n_plus_char) {
    my_check("\na");
}

TEST(correctness, chars_back_slash_n_plus_chars) {
    my_check("ajtjglt\nnln");
}

TEST(correctness, chars_tabs) {
    my_check("a\tjtj\tglt\tnln\t");
}

TEST(correctness, spaces) {
    my_check("       ");
}

TEST(correctness, one_hundred_a) {
    std::string input(100, 'a');
    my_check(input);
}

TEST(correctness, small_fuzzing_a_z) {
    srand(0);
    for (int i = 0; i < 100; ++i) {
        std::string input;
        for (int j = 0; j < 100; ++j) {
            input += ('a' + static_cast<unsigned char>(rand() % 26));
        }
        my_check(input);
    }
}

TEST(correctness, small_fuzzing_0_256) {
    srand(0);
    for (int i = 0; i < 100; ++i) {
        std::string input;
        for (int j = 0; j < 100; ++j) {
            input += static_cast<unsigned char>(rand() % 256);
        }
        my_check(input);
    }
}
