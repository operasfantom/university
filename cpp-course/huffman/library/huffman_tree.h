#ifndef HUFFMAN_LIBRARY_H
#define HUFFMAN_LIBRARY_H

#include <ostream>
#include <vector>

//const size_t SYMBOLS = std::numeric_limits<huffman_tree::symbol_t>::max();
const size_t SYMBOLS = 256;

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
        return val < rhs.val;
    }
};

class huffman_tree {
public:
    typedef char symbol_t;
    typedef std::basic_string<symbol_t> string_t;
private:
    std::vector<int64_t> cnt;
    std::vector<string_t> code;

    Node<symbol_t> *root = nullptr;

    string_t dictionary;
    string_t path;

    void dfs(Node<symbol_t> *vertex, string_t &current_code, string_t &dictionary, string_t &path);

    void build_tree(Node<symbol_t> *vertex, symbol_t *&, symbol_t *&);

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

    string_t get_path();

    string_t get_dictionary();

    string_t get_code(symbol_t);

    void set_path(string_t const &);

    void set_dictionary(string_t const &);

    void decoding();

    std::pair<symbol_t, bool> transition(symbol_t c);

};

#endif