//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "interval.h"

Interval::Interval(int start, int end, bool finite) : start(start), end(end), finite(finite) {}

std::string Interval::to_string() const {
    return "[" + std::to_string(this->start) + ", " + std::to_string(this->end) + "]";
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

void Interval::setStart(int st) { start = st; }

int Interval::getEnd() const { return end; }

void Interval::setEnd(int en) { end = en; }

bool Interval::isFinite() const { return finite; }

void Interval::setFinite(bool fin) { finite = fin; }

bool Interval::containsZero() const {
//    if (start == 0 or end == 0)
//        return true;
//    else
//        return false;
    return (start<=0 and 0<=end);
}