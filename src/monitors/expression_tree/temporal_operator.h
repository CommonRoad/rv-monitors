#pragma once

#include "memory"
#include <iostream>
#include <utility>

#include "interval.h"

/**
 * Representation of temporal operator expression.
 */
class TemporalOperator {
  private:
    std::shared_ptr<Interval> interval; //**< Temporal interval.  */

  public:
    /**
     * Constructor.
     *
     * @param interval Temporal interval.
     */
    explicit TemporalOperator(std::shared_ptr<Interval> interval) : interval(std::move(interval)) {}

    /**
     * Getter for interval.
     *
     * @return Interval.
     */
    [[nodiscard]] const std::shared_ptr<Interval> &getInterval() const { return interval; }

    /**
     * Copy constructor of temporal operator.
     *
     * @param old Temporal operator object which should be copied.
     */
    TemporalOperator(const TemporalOperator &old);
};
