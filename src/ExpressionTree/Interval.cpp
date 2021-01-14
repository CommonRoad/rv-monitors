//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "Interval.h"

Interval::Interval(int start, int end, bool finite) : start(start), end(end), finite(finite) {}

std::string Interval::to_string() const {
    return "[" + std::to_string(this->start) + ", " + std::to_string(this->end) + "]";
}

