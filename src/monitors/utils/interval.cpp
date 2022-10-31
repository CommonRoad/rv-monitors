//
// Created by sebastian on 19.09.22.
//

#include "interval.h"
#include "cassert"

IntervalTemporalOperator::IntervalTemporalOperator(size_t start, size_t end, bool finite, bool empty)
    : start(start), end(end), finite(finite), empty(empty) {
    assert((!empty or !finite) && "Invalid temporal operator interval.");
}

IntervalTemporalOperator::IntervalTemporalOperator(size_t start, size_t end)
    : start(start), end(end), finite(true), empty(false) {}

IntervalTemporalOperator::IntervalTemporalOperator(size_t start) : start(start), finite(false), empty(false) {}

IntervalTemporalOperator::IntervalTemporalOperator() : start(0), finite(false), empty(true) {}

IntervalTemporalOperator::IntervalTemporalOperator(const std::shared_ptr<Interval> &interval) {
    start = static_cast<size_t>(interval->getStart());
    finite = interval->isFinite();
    empty = interval->isEmpty();
    if (interval->isFinite())
        end = interval->getEnd();
}

size_t IntervalTemporalOperator::getStart() const { return start; }

size_t IntervalTemporalOperator::getEnd() const {
    assert((finite) && "Invalid interval access: Interval is infinite.");
    return end.value();
}

bool IntervalTemporalOperator::isFinite() const { return finite; }

bool IntervalTemporalOperator::isEmpty() const { return empty; }
