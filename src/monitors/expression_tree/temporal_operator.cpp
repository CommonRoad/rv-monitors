//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "temporal_operator.h"

TemporalOperator::TemporalOperator(const TemporalOperator &old) {
    if (old.interval->isFinite())
        interval = std::make_shared<Interval>(old.interval->getStart(), old.interval->getEnd());
    else if (old.interval->isEmpty())
        interval = std::make_shared<Interval>();
    else
        interval = std::make_shared<Interval>(old.getInterval()->getStart());
}