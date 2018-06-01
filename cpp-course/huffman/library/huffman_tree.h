#ifndef HUFFMAN_LIBRARY_H
#define HUFFMAN_LIBRARY_H

#include <ostream>
#include <vector>
#include "bit_container.h"

const size_t SYMBOLS = 256;

class huffman_tree {
public:
    typedef unsigned char symbol_t;
    typedef std::basic_string<symbol_t> string_t;
private:
    template <typename S>
    struct Node {
        Node *left = nullptr, *right = nullptr;

        int64_t val = 0;
        S symbol = 0;

        Node() = default;

        explicit Node(Node *first, Node *second) : left(first), right(second) {
            if (left) val += left->val;
            if (right) val += right->val;
        }

        explicit Node(S symbol, int64_t val) : val(val), symbol(symbol) {}

        bool operator<(const Node &rhs) const {
            if (val == rhs.val){
                return symbol < rhs.symbol;
            }
            return val < rhs.val;
        }
    };

    std::vector<int64_t> cnt;
    std::vector<bit_container> code;

    Node<symbol_t> *root = nullptr;

    string_t dictionary;
    bit_container path;

    void dfs(Node<symbol_t> *vertex, bit_container &current_code, string_t &dictionary, bit_container &path);

    void build_tree(Node<symbol_t> *vertex, bit_container::bool_iterator &, symbol_t *&);

    void delete_tree(Node<symbol_t>* vertex);

    bool is_leaf(Node<symbol_t> *pNode);

    Node<symbol_t>* current_node = nullptr;

    struct compare_nodes{
        bool operator()(Node<symbol_t> *first, Node<symbol_t> *second){
            return first->val > second->val;
        }
    };
public:

    huffman_tree();

    virtual ~huffman_tree();

    void add(symbol_t);

    void encoding();

    bit_container get_path();

    string_t get_dictionary();

    bit_container get_code(symbol_t);

    void set_path(const bit_container &);

    void set_dictionary(string_t const &);

    void decoding();

    std::pair<symbol_t, bool> transition(bool c);

    size_t get_text_length();
};

#endif //HUFFMAN_LIBRARY_H