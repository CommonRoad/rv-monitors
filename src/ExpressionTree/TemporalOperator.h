//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_TEMPORALOPERATOR_H
#define RV_MONITORS_TEMPORALOPERATOR_H

#include "memory"
#include <iostream>
#include <utility>

#include "Interval.h"

class TemporalOperator {
  private:
    std::shared_ptr<Interval> interval;

  public:
    explicit TemporalOperator(std::shared_ptr<Interval> interval) : interval(std::move(interval)) {}

    void reduceInterval() { interval->reduceInterval(); }

    [[nodiscard]] const std::shared_ptr<Interval> &getInterval() const { return interval; }

    TemporalOperator(const TemporalOperator &old) {
        interval =
            std::make_shared<Interval>(old.interval->getStart(), old.interval->getEnd(), old.interval->isFinite());
    }
};

#endif // RV_MONITORS_TEMPORALOPERATOR_H
