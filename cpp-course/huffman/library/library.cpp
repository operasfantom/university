#include <numeric>
#include <algorithm>
#include <queue>
#include <fstream>
#include "library.h"


void huffman_tree::add(symbol_t c) {
    ++cnt[c];
}

huffman_tree::huffman_tree() {
    cnt.resize(SYMBOLS, 0);
    code.resize(SYMBOLS);
}


void huffman_tree::dfs(Node<symbol_t> *vertex, string_t &current_code, string_t &dictionary, string_t &path) {
    if (vertex->left == nullptr && vertex->right == nullptr) {
        dictionary.push_back(vertex->symbol);
        code[vertex->symbol] = current_code;
    } else {
        if (vertex->left != nullptr) {
            path += 'L';
            current_code += '0';
            dfs(vertex->left, current_code, dictionary, path);
            current_code.pop_back();
        }
        if (vertex->right != nullptr) {
            path += 'R';
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

void huffman_tree::encoding() {
    std::priority_queue<Node<symbol_t> *> q;
    for (size_t i = 0; i < SYMBOLS; ++i) {
        if (cnt[i] > 0) {
            Node<symbol_t> *tmp = new Node<symbol_t>(static_cast<symbol_t>(i), cnt[i]);
            q.push(tmp);
        }
    }

    if (q.size() == 0){
        return;//empty file
    }
    while (q.size() > 1) {
        Node<symbol_t> *first_min = q.top();
        q.pop();
        Node<symbol_t> *second_min = q.top();
        q.pop();

        Node<symbol_t> *union_node = new Node<symbol_t>(first_min, second_min);
        q.push(union_node);
    }
    root = q.top();

    string_t current_code;
    dfs(root, current_code, dictionary, path);

    path.pop_back();
}

void huffman_tree::decoding() {
    Node<symbol_t> *current_node = root = new Node<symbol_t>();
    symbol_t* current_symbol = &dictionary[0];
    symbol_t* current_step = &path[0];
    build_tree(current_node, current_step, current_symbol);

    huffman_tree::current_node = root;
}

std::pair<wchar_t, bool> huffman_tree::transition(symbol_t c) {
    switch (c){
        case '0':{
            current_node = current_node->left;
            break;
        }
        case '1':{
            current_node = current_node->right;
            break;
        }
        default:{
            //error
        }
    }
    if (is_leaf(current_node)){
        symbol_t result = current_node->symbol;
        current_node = root;
        return std::make_pair(result, true);
    }
    return std::make_pair(0, false);
}

bool huffman_tree::is_leaf(Node<huffman_tree::symbol_t> *vertex) {
    return vertex->left == nullptr && vertex->right == nullptr;
}

void huffman_tree::build_tree(Node<symbol_t> *vertex, wchar_t *&c, wchar_t *&current_symbol) {
    if (*c == 'L') {
        build_tree(vertex->left = new Node<symbol_t>(), ++c, current_symbol);
    }
    if (*c == 'R'){
        build_tree(vertex->right = new Node<symbol_t>(), ++c, current_symbol);
    }
    if (*c == 'U'){
        if (is_leaf(vertex)) {
            vertex->symbol = *(current_symbol++);
        }
        ++c;
    }
}