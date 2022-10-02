//
// Created by Sebastian Maierhofer on 12.01.21.
//

#include "interval.h"

#include "atomic/predicate.h"
#include "cassert"
#include <utility>

Interval::Interval(int start, int end) : start(start), end(end), finite(true), empty(false), dynamic(false) {}

Interval::Interval() : start(0), finite(false), empty(true), dynamic(false) {}

Interval::Interval(int start) : start(start), finite(false), empty(false), dynamic(false) {}

Interval::Interval(std::array<std::shared_ptr<Predicate>, 2> dynamicInterval)
    : finite(true), empty(false), dynamicInterval(std::move(dynamicInterval)), dynamic(true) {}

std::string Interval::to_string() const {
    std::string endStr{};
    std::string startStr{};
    if (!dynamic)
        startStr = std::to_string(this->start);
    else
        startStr = dynamicInterval.at(0)->to_string();
    if (!empty) {
        if (!finite and !dynamic)
            endStr = "inf";
        else if (dynamic)
            endStr = dynamicInterval.at(1)->to_string();
        else
            endStr = std::to_string(end.value());
    }
    return "[" + startStr + ", " + endStr + "]";
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

bool Interval::isDynamic() const { return dynamic; }

const std::array<std::shared_ptr<Predicate>, 2> &Interval::getDynamicInterval() const { return dynamicInterval; }
