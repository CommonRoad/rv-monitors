//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "Interval.h"

Interval::Interval(int start, int end, bool finite) : start(start), end(end), finite(finite) {}

std::string Interval::to_string() const {
    return "[" + std::to_string(this->start) + ", " + std::to_string(this->end) + "]";
}

bool Interval::reduceInterval(){
    if (!finite)
        return true;
    if (end > 0)
        end--;
    if (end == 0)
        return false;
    else
        return true;
}

int Interval::getStart() const { return start; }

void Interval::setStart(int st) { start = st; }

int Interval::getAnEnd() const { return end; }

void Interval::setAnEnd(int anEnd) { end = anEnd; }

bool Interval::isFinite() const { return finite; }

void Interval::setFinite(bool fin) { finite = fin; }

bool Interval::containsZero() const {
    if (start == 0 or end == 0)
        return true;
    else
        return false;
}