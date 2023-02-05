#include "monitors/utils/lazy_bool.h"

#include <utility>

lazy_bool::lazy_bool(std::function<bool()> valueFetcher) : valueFetcher(std::move(valueFetcher)) {}

bool lazy_bool::get() const {
    if (not initialized) {
        value = valueFetcher();
        initialized = true;
    }
    return value;
}

std::string lazy_bool::to_string() const {
    if (initialized) {
        return value ? "true" : "false";
    }
    return "Not initialized";
}

std::ostream &operator<<(std::ostream &os, const lazy_bool &lazyBool) { return os << lazyBool.to_string(); }

lazy_bool::lazy_bool(bool value) : value(value), initialized(true) {}
