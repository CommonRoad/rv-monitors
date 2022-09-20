//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "interval.h"

Interval::Interval(int start, int end, bool finite, bool empty)
    : start(start), end(end), finite(finite), empty(empty) {}

std::string Interval::to_string() const {
    std::string endStr{};
    if (!empty) {
        if (!finite)
            endStr = "inf";
        else
            endStr = std::to_string(end);
    }
    return "[" + std::to_string(this->start) + ", " + endStr + "]";
}

void Interval::reduceInterval() {
    if (!finite)
        start--;
    else {
        end--;
        start--;
    }
}

int Interval::getStart() const { return start; }

int Interval::getEnd() const { return end; }

bool Interval::isFinite() const { return finite; }

int Interval::getLength() const { return end - start; }

bool Interval::isEmpty() const { return empty; }
