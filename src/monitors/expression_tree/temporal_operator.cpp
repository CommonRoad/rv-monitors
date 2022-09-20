//
// Created by Sebastian Maierhofer on 14.01.21.
//

#include "temporal_operator.h"

TemporalOperator::TemporalOperator(const TemporalOperator &old) {
    interval = std::make_shared<Interval>(old.interval->getStart(), old.interval->getEnd(), old.interval->isFinite(),
                                          old.interval->isEmpty());
}