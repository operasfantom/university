#include <numeric>
#include <algorithm>
#include <queue>
#include <fstream>
#include "huffman_tree.h"
#include <exception>
#include <bits/shared_ptr.h>

void huffman_tree::add(symbol_t c) {
    ++cnt[c];
}

huffman_tree::huffman_tree() {
    cnt.resize(SYMBOLS, 0);
    code.resize(SYMBOLS);
}


void huffman_tree::dfs(Node<symbol_t> *vertex, container &current_code, string_t &dictionary, container &path) {
    if (is_leaf(vertex)) {
        dictionary.push_back(vertex->symbol);
        code[vertex->symbol] = current_code;
    } else {
        if (vertex->left != nullptr) {
//            path += 'D';
            path.wide_push_back(true);
            current_code.push_back(false);
            dfs(vertex->left, current_code, dictionary, path);
            current_code.pop_back();
        }
        if (vertex->right != nullptr) {
//            path += 'D';
            path.wide_push_back(true);
            current_code.push_back(true);
            dfs(vertex->right, current_code, dictionary, path);
            current_code.pop_back();
        }
    }
//    path += 'U';
    path.wide_push_back(false);
}

huffman_tree::container const &huffman_tree::get_path() const {
    return path;
}

huffman_tree::string_t const & huffman_tree::get_dictionary() const {
    return dictionary;
}

huffman_tree::container const &huffman_tree::get_code(symbol_t c) {
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

void huffman_tree::set_path(const container &path) {
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

    huffman_tree::container current_code;
    dfs(root, current_code, dictionary, path);

    path.pop_back();
}

//Pre: set_path() && set_dictionary()
void huffman_tree::decoding() {
    Node<symbol_t> *current_node = root = new Node<symbol_t>();
    auto* current_symbol = &dictionary[0];
    container::bool_iterator current_step(path);
    try {
        build_tree(current_node, current_step, current_symbol);
    } catch (std::exception &e) {
        throw;
    }

    if (!current_step.is_end()) {
        throw std::invalid_argument("odd symbols in path");
    }
    huffman_tree::current_node = root;
}

bool huffman_tree::transition(char c, char *buffer) {
    if (c) {
        current_node = current_node->right;
        if (!current_node) {
            throw std::invalid_argument("received unacceptable symbol");
        }
    } else {
        current_node = current_node->left;
        if (!current_node) {
            throw std::invalid_argument("received unacceptable symbol");
        }
    }
    if (is_leaf(current_node)) {
        *buffer = current_node->symbol;
        current_node = root;
        return true;
    }
    return false;
}

bool huffman_tree::is_leaf(Node<huffman_tree::symbol_t> *vertex) {
    return vertex->left == nullptr && vertex->right == nullptr;
}

void huffman_tree::build_tree(Node<symbol_t> *vertex, container::bool_iterator &step, symbol_t *&current_symbol) {
    if (!step.is_end() && step.get() == true) {
        build_tree(vertex->left = new Node<symbol_t>(), ++step, current_symbol);
    }
    if (!step.is_end() && step.get() == true) {
        build_tree(vertex->right = new Node<symbol_t>(), ++step, current_symbol);
    }
    if (!step.is_end() && step.get() == false) {
        if (is_leaf(vertex)) {
            vertex->symbol = *(current_symbol++);
        }
        ++step;
    }
}

size_t huffman_tree::get_text_length() {
    size_t result = 0;
    for (size_t i = 0; i < SYMBOLS; ++i) {
        result += cnt[i] * get_code(i).size();
    }
    return result;
}
