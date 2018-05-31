#include <numeric>
#include <algorithm>
#include <queue>
#include <fstream>
#include "huffman_tree.h"
#include <exception>

void huffman_tree::add(symbol_t c) {
    ++cnt[c];
}

huffman_tree::huffman_tree() {
    cnt.resize(SYMBOLS, 0);
    code.resize(SYMBOLS);
}


void huffman_tree::dfs(Node<symbol_t> *vertex, string_t &current_code, string_t &dictionary, string_t &path) {
    if (is_leaf(vertex)) {
        dictionary.push_back(vertex->symbol);
        code[vertex->symbol] = current_code;
    } else {
        if (vertex->left != nullptr) {
            path += 'D';
            current_code += '0';
            dfs(vertex->left, current_code, dictionary, path);
            current_code.pop_back();
        }
        if (vertex->right != nullptr) {
            path += 'D';
            current_code += '1';
            dfs(vertex->right, current_code, dictionary, path);
            current_code.pop_back();
        }
    }
    path += 'U';
}

huffman_tree::string_t huffman_tree::get_path() {
    return path;
}

huffman_tree::string_t huffman_tree::get_dictionary() {
    return dictionary;
}

huffman_tree::string_t huffman_tree::get_code(symbol_t c) {
    return code[c];
}

void huffman_tree::delete_tree(Node<symbol_t>* vertex){
    if (vertex == nullptr) return;
    delete_tree(vertex->left);
    delete_tree(vertex->right);
    delete vertex;
}

huffman_tree::~huffman_tree() {
    delete_tree(root);
}

void huffman_tree::set_path(string_t const &path) {
    this->path = path;
}

void huffman_tree::set_dictionary(string_t const &dictionary) {
    this->dictionary = dictionary;
}

//Pre: assigned cnt[]
void huffman_tree::encoding() {
    std::priority_queue<Node<symbol_t> *, std::vector<Node<symbol_t> *>, compare_nodes> q;
    for (size_t i = 0; i < SYMBOLS; ++i) {
        if (cnt[i] > 0) {
            auto *tmp = new Node<symbol_t>(static_cast<symbol_t>(i), cnt[i]);
            q.push(tmp);
        }
    }

    if (q.empty()) {
        return;//empty file
    }
    if (q.size() == 1) {
        Node<symbol_t> *vertex = q.top();
        q.pop();
        q.push(new Node<symbol_t>(vertex, nullptr));
    }

    while (q.size() > 1) {
        Node<symbol_t> *first_min = q.top();
        q.pop();
        Node<symbol_t> *second_min = q.top();
        q.pop();

        auto *union_node = new Node<symbol_t>(first_min, second_min);
        q.push(union_node);
    }
    root = q.top();

    string_t current_code;
    dfs(root, current_code, dictionary, path);

    path.pop_back();
}

//Pre: set_path() && set_dictionary()
void huffman_tree::decoding() {
    auto it = std::find_if(path.begin(), path.end(), [](symbol_t c) { return c != 'D' && c != 'U'; });
    if (it != path.end()) {
        throw std::invalid_argument("incorrect symbol in path: " + *it);
    }

    Node<symbol_t> *current_node = root = new Node<symbol_t>();
    symbol_t* current_symbol = &dictionary[0];
    symbol_t* current_step = &path[0];
    build_tree(current_node, current_step, current_symbol);

    if (string_t::iterator(current_step) != path.end()) {
        throw std::invalid_argument("odd symbols in path");
    }
    huffman_tree::current_node = root;
}

std::pair<huffman_tree::symbol_t, bool> huffman_tree::transition(symbol_t c) {
    switch (c){
        case '0':{
            current_node = current_node->left;
            if (!current_node) {
                throw std::invalid_argument("received unacceptable symbol");
            }
            break;
        }
        case '1':{
            current_node = current_node->right;
            if (!current_node) {
                throw std::invalid_argument("received unacceptable symbol");
            }
            break;
        }
        default:{
            throw std::invalid_argument("received invalid symbol, expected 0 or 1");
        }
    }
    if (is_leaf(current_node)) {
        symbol_t result = current_node->symbol;
        current_node = root;
        return std::make_pair(result, true);
    }
    return std::make_pair(0, false);
}

bool huffman_tree::is_leaf(Node<huffman_tree::symbol_t> *vertex) {
    return vertex->left == nullptr && vertex->right == nullptr;
}

void huffman_tree::build_tree(Node<symbol_t> *vertex, symbol_t *&c, symbol_t *&current_symbol) {
    if (*c == 'D') {
        build_tree(vertex->left = new Node<symbol_t>(), ++c, current_symbol);
    }
    if (*c == 'D') {
        build_tree(vertex->right = new Node<symbol_t>(), ++c, current_symbol);
    }
    if (*c == 'U'){
        if (is_leaf(vertex)) {
            vertex->symbol = *(current_symbol++);
        }
        ++c;
    }
}