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
    if (start == 0 or end == 0)
        return true;
    else
        return false;
  //  return (start<=0 and end>=0);
    // return (start>=-1 or end>=-1); // to include the interval extremes
//    return (start>=0 or end>=0); // not to include the interval extremes
}
void Interval::reduceIntervalByTimeStep(size_t timeStep) {
    start -= timeStep;
    end -= timeStep;
}
void Interval::increaseIntervalByTimeStep(size_t timeStep) {
    start += timeStep;
    end += timeStep;
}
bool Interval::getContainsZero(size_t index, bool stop_checking) const {
    if (!stop_checking){
        if (static_cast<unsigned long>(start) + index == 0 or static_cast<unsigned long>(end) + index == 0)
            return true;
        else
            return false;
    }
    else {
        return false;
    }
}
