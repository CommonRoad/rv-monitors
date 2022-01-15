//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_TEMPORAL_OPERATOR_H
#define RV_MONITORS_TEMPORAL_OPERATOR_H

#include "memory"
#include <iostream>
#include <utility>

#include "interval.h"

class TemporalOperator {
  private:
    std::shared_ptr<Interval> interval;

  public:
    explicit TemporalOperator(std::shared_ptr<Interval> interval) : interval(std::move(interval)) {}

    void reduceInterval() { interval->reduceInterval(); }

    void reduceIntervalByTimeStep(size_t timeStep) { interval->reduceIntervalByTimeStep(timeStep); }

    void increaseIntervalByTimeStep(size_t timeStep) { interval->increaseIntervalByTimeStep(timeStep); }


    [[nodiscard]] const std::shared_ptr<Interval> &getInterval() const { return interval; }

    TemporalOperator(const TemporalOperator &old) {
        interval =
            std::make_shared<Interval>(old.interval->getStart(), old.interval->getEnd(), old.interval->isFinite());
    }
};

#endif // RV_MONITORS_TEMPORAL_OPERATOR_H
