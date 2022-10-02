//
// Created by sebastian on 19.09.22.
//

#include "interval.h"
#include "cassert"
#include "interval_function.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include <utility>
#include "monitors/expression_tree/atomic/predicate.h"

size_t Hydra2IntervalTemporalOperator::getStart() const { return start; }

size_t Hydra2IntervalTemporalOperator::getEnd() const {
    assert((finite) && "Invalid interval access: Interval is infinite.");
    return end.value();
}
Hydra2IntervalTemporalOperator::Hydra2IntervalTemporalOperator(
    std::tuple<std::shared_ptr<PredicateMonitor>, std::shared_ptr<IntervalFunction>> dynamicInterval)
    : dynamicInterval(std::move(dynamicInterval)), dynamic(true) {}

Hydra2IntervalTemporalOperator::Hydra2IntervalTemporalOperator(const std::shared_ptr<Interval> &interval) {
    if (interval->isDynamic()) {
        dynamic = true;
        finite = true;
        empty = false;
        std::get<0>(dynamicInterval) =
            std::make_shared<PredicateMonitor>(Formula(interval->getDynamicInterval().at(0)), 0, false);
        std::get<1>(dynamicInterval) =
            std::make_shared<IntervalFunction>(Formula(interval->getDynamicInterval().at(1)));
    } else {
        dynamic = false;
        start = static_cast<size_t>(interval->getStart());
        finite = interval->isFinite();
        empty = interval->isEmpty();
        if (interval->isFinite())
            end = interval->getEnd();
    }
}

void Hydra2IntervalTemporalOperator::updateDynamicInterval(size_t stepNumber) {
    if (std::get<0>(dynamicInterval)->concreteStep(0, stepNumber).value) {
        auto interv{std::get<1>(dynamicInterval)->concreteStep(stepNumber)};
        start = interv[0];
        end = interv[1];
        finite = true;
    } else {
        finite = false;
        start = 0;
        end = {};
    }
}

Hydra2IntervalTemporalOperator::Hydra2IntervalTemporalOperator(size_t start, size_t end, bool finite, bool empty)
    : IntervalTemporalOperator(start, end, finite, empty) {}

Hydra2IntervalTemporalOperator::Hydra2IntervalTemporalOperator(size_t start, size_t end)
    : IntervalTemporalOperator(start, end), dynamic(false) {}

Hydra2IntervalTemporalOperator::Hydra2IntervalTemporalOperator(size_t start) : IntervalTemporalOperator(start), dynamic(false) {}

std::string Hydra2IntervalTemporalOperator::to_string() {
    if(empty)
        return "";
    std::string endStr{};
    std::string startStr{};
    if (!dynamic)
        startStr = std::to_string(this->start);
    else
        startStr = std::get<0>(dynamicInterval)->to_string();
    if (!empty) {
        if (!finite and !dynamic)
            endStr = "inf";
        else if (dynamic)
            endStr = std::get<1>(dynamicInterval)->to_string();
        else
            endStr = std::to_string(end.value());
    }
    return "[" + startStr + ", " + endStr + "]";
}

void Hydra2IntervalTemporalOperator::setTrace(const Trace& trace) {
    if(dynamic) {
        std::get<0>(dynamicInterval)->setTrace(trace);
        std::get<1>(dynamicInterval)->setTrace(trace);
    }
}
