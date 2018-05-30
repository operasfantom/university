#ifndef HUFFMAN_LIBRARY_H
#define HUFFMAN_LIBRARY_H

#include <ostream>
#include <vector>

//const size_t SYMBOLS = std::numeric_limits<wchar_t>::max();
const size_t SYMBOLS = 512;

template <typename S>
struct Node {
    Node *left = nullptr, *right = nullptr;

    int64_t val = 0;
    S symbol = 0;

    Node() = default;

    explicit Node(Node *first, Node *second) : left(first), right(second) {
        val = left->val + right->val;
    }

    explicit Node(S symbol, int64_t val) : val(val), symbol(symbol) {}

    bool operator<(const Node &rhs) const {
        return val < rhs.val;
    }
};

class huffman_tree {
    typedef wchar_t symbol_t;
    typedef std::wstring string_t;

    std::vector<int64_t> cnt;
    std::vector<string_t> code;

    Node<symbol_t> *root = nullptr;

    string_t dictionary;
    string_t path;

    void dfs(Node<symbol_t> *vertex, string_t &current_code, string_t &dictionary, string_t &path);

    void build_tree(Node<symbol_t> *vertex, wchar_t *&, wchar_t *&);

    void delete_tree(Node<symbol_t>* vertex);

    bool is_leaf(Node<symbol_t> *pNode);

    Node<symbol_t>* current_node = nullptr;

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

    std::pair<wchar_t, bool> transition(symbol_t c);

};

#endif