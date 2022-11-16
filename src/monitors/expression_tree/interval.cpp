//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "interval.h"
#include "cassert"

Interval::Interval(int start, int end) : start(start), end(end), finite(true), empty(false) {}

Interval::Interval() : start(0), finite(false), empty(true) {}

Interval::Interval(int start) : start(start), finite(false), empty(false) {}

std::string Interval::to_string() const {
    std::string endStr{};
    if (!empty) {
        if (!finite)
            endStr = "inf";
        else
            endStr = std::to_string(end.value());
        return "[" + std::to_string(this->start) + ", " + endStr + "]";
    }
    return "";
}

int Interval::getStart() const { return start; }

int Interval::getEnd() const {
    assert((finite) && "Invalid interval access: Interval is infinite.");
    return end.value();
}

bool Interval::isFinite() const { return finite; }

int Interval::getLength() const {
    assert((finite) && "Invalid length computation: Interval is infinite.");
    return end.value() - start;
}

bool Interval::isEmpty() const { return empty; }
