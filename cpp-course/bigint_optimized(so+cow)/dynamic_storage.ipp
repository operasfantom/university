#ifndef BIGINT_DYNAMIC_STORAGE_HPP
#define BIGINT_DYNAMIC_STORAGE_HPP

#include "dynamic_storage.h"

template<typename T>
dynamic_storage<T>::dynamic_storage() noexcept {
    ref_counter = 1;
    data = (T *) malloc(INITIAL_CAPACITY * sizeof(T));
    if (data == nullptr){
        std::cerr << "couldn't create dynamic_storage()";
        exit(0);
    }
    //std::cerr << "malloc (val)" << storage << '\n';
}

template<typename T>
dynamic_storage<T>::dynamic_storage(dynamic_storage const &other) noexcept {
    ref_counter = 1;
    capacity = other.capacity;
    data = (T *) malloc(capacity * sizeof(T));
    if (data == nullptr){
        std::cerr << "couldn't create dynamic_storage()";
        exit(0);
    }
    memcpy(data, other.data, capacity * sizeof(T));
}

template<typename T>
dynamic_storage<T>::~dynamic_storage() noexcept {
    free(data);
}


#endif //BIGINT_DYNAMIC_STORAGE_HPP
