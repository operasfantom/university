//
// Created by operasfantom on 31.05.18.
//

#include "binary_io.h"


size_t read_size_t(std::istream &ifs) {
    size_t result;
    ifs.read(reinterpret_cast<char *>(&result), sizeof(result));
    return result;
}

huffman_tree::string_t read_extended_string(std::istream &ifs) {
    huffman_tree::string_t result;
    size_t n = read_size_t(ifs);
    result.resize(n);
    ifs.read(reinterpret_cast<char *>(&result[0]), n);
    return result;

}

huffman_tree::container read_extended_bit_container(std::istream &ifs) {
    size_t n = read_size_t(ifs);
    huffman_tree::container result(n);
    ifs.read(reinterpret_cast<char *>(&result[0]), result.char_blocks_count());
    return result;
}

void print(std::ofstream &ofs, size_t n) {
    ofs.write(reinterpret_cast<const char *>(&n), sizeof(n));
}

void print(std::ofstream &ofs, huffman_tree::string_t const &s) {
    ofs.write(reinterpret_cast<const char *>(s.data()), s.length() * sizeof(s[0]));
}

void print(std::ofstream &ofs, huffman_tree::container const &s) {
    ofs.write(reinterpret_cast<const char *>(s.data()), s.char_blocks_count());
}

void print_extended(std::ofstream &ofs, huffman_tree::string_t const &s) {
    print(ofs, s.size());
    print(ofs, s);
}

void print_extended(std::ofstream &ofs, huffman_tree::container const &s) {
    print(ofs, s.size());
    print(ofs, s);
}

void print(std::ostream &ofs, char *first, char *last) {
    ofs.write(first, last - first);
}

//---

binary_file::binary_file(std::string const &file_name, std::ios::openmode mode) : mode_in(mode == std::ios::in),
                                                                                  stream(file_name,
                                                                                         std::ios::binary | mode) {
    if (!stream.is_open()) {
        throw std::runtime_error("could not open file:" + file_name);
    }
}

char binary_file::next_char() {
    return *(position++);
}

bool binary_file::has_next_char() {
    if (position == last) {
        read_buffer();
        size_t have_been_read = static_cast<size_t>(stream.gcount());
        last = (buffer + have_been_read);
        position = buffer;
    }
    return position != last;
}

void binary_file::read_buffer() {
    stream.read(buffer, buffer_size);
}

void binary_file::initialize_last() {
    last = (mode_in ? buffer : buffer + buffer_size);
}

void binary_file::set_buffer(char *begin, size_t n) {
    buffer_size = n;

    buffer = begin;
    position = buffer;
    initialize_last();
}

char *binary_file::get_target() {
    return position;
}

bool binary_file::full() {
    return position == last;
}

std::iostream &binary_file::get_stream() {
    return stream;
}

void binary_file::dump(std::ostream &out) {
    out.write(buffer, position - buffer);
    position = buffer;
    initialize_last();
}

void binary_file::move_forward_position() {
    ++position;
}

binary_file::~binary_file() {
    /*operator delete(buffer);
    operator delete(position);
    operator delete(last);*/
}
