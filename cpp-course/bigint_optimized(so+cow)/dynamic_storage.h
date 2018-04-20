#ifndef BIGINT_DYNAMIC_STORAGE_H
#define BIGINT_DYNAMIC_STORAGE_H

template <typename T>
struct dynamic_storage {
    size_t ref_counter;
    size_t capacity;

    T* data;

    dynamic_storage();

    dynamic_storage(dynamic_storage const &other);

    ~dynamic_storage();
};



#include "dynamic_storage.hpp"

#endif //BIGINT_DYNAMIC_STORAGE_H
