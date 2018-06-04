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
    typedef uint64_t block_t;
    typedef bit_container<block_t> container;

    static const size_t BLOCK_SIZE = sizeof(block_t);
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
    std::vector<container> code;

    Node<symbol_t> *root = nullptr;

    string_t dictionary;
    container path;

    void dfs(Node<symbol_t> *vertex, container &current_code, string_t &dictionary, container &path);

    void build_tree(Node<symbol_t> *vertex, container::bool_iterator &, symbol_t *&);

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

    container const& get_path() const;

    string_t const & get_dictionary() const;

    container const &get_code(symbol_t);

    void set_path(const container &);

    void set_dictionary(string_t const &);

    void decoding();

    void transition(bool c, char *&buffer);

    size_t get_text_length();
};

#endif //HUFFMAN_LIBRARY_H