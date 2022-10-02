#include "lazy_function.h"

#include <utility>

template <class T> LazyFunction<T>::LazyFunction(std::function<T()> valueFetcher) : valueFetcher(std::move(valueFetcher)) {}

template <class T> T LazyFunction<T>::get() const {
    if (not initialized) {
        value = valueFetcher();
        initialized = true;
    }
    return value;
}

template <> std::string LazyFunction<bool>::to_string() const {
    if (initialized) {
        return value ? "true" : "false";
    }
    return "Not initialized";
}


template <class T> LazyFunction<T>::LazyFunction(T value) : value(value), initialized(true) {}

template <> LazyFunction<bool>::operator bool() { return get(); }

template class LazyFunction<bool>; //required otherwise compiler won't instantiate template
template class LazyFunction<std::array<size_t, 2>>; //required otherwise compiler won't instantiate template