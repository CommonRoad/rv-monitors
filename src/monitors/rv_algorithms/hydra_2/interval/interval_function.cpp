//
// Created by sebastian on 01.10.22.
//
#include "interval_function.h"
#include <limits>

std::array<size_t, 2> IntervalFunction::concreteStep(size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        auto event{trace.getIntervalEvent(stepNumber)};
        return event.getValue(formula.to_string());
    } else
        return {0, std::numeric_limits<size_t>::max()};
}

void IntervalFunction::setTrace(const Trace &tra) { trace = tra; }

std::string IntervalFunction::to_string() { return formula.to_string(); }
