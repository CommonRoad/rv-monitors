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

IntervalTemporalOperator::IntervalTemporalOperator() : start(0), end(10000), finite(false), empty(true) {}

size_t IntervalTemporalOperator::getStart() const { return start; }

size_t IntervalTemporalOperator::getEnd() const { return end; }

bool IntervalTemporalOperator::isFinite() const { return finite; }

bool IntervalTemporalOperator::isEmpty() const { return empty; }
