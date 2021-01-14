//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_TEMPORALOPERATOR_H
#define RV_MONITORS_TEMPORALOPERATOR_H

#include <utility>
#include "memory"

#include "ExpressionTree/Atomic/Interval.h"

class TemporalOperator {
private:
    std::shared_ptr<Interval> interval;

public:
    explicit TemporalOperator(std::shared_ptr<Interval> interval) : interval(std::move(interval)) {}

    bool reduceInterval(){
        return interval->reduceInterval();
    }
};


#endif //RV_MONITORS_TEMPORALOPERATOR_H
