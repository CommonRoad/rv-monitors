#pragma once

#include "monitors/trace/trace.h"
#include "monitors/utils/interval.h"
#include <tuple>

class PredicateMonitor;
class IntervalFunction;

using HydraIntervalTriggers = std::tuple<std::shared_ptr<PredicateMonitor>, std::shared_ptr<IntervalFunction>>;

/**
 * Representation of interval for temporal operators.
 */
struct Hydra2IntervalTemporalOperator : public IntervalTemporalOperator {
  public:
    /**
     * Constructor with all attributes.
     *
     * @param start Start of interval.
     * @param end End of interval.
     * @param finite Boolean indicating whether interval is finite.
     * @param empty Boolean indicating whether interval is empty, i.e. infinite interval.
     */
    Hydra2IntervalTemporalOperator(size_t start, size_t end, bool finite, bool empty);

    /**
     * Constructor defining only start and end. Therefore, interval is not empty and finite.
     *
     * @param start Start of interval.
     * @param end End of interval.
     */
    Hydra2IntervalTemporalOperator(size_t start, size_t end);

    /**
     * Constructor defining only start. End is infinite.
     *
     * @param start Start of interval.
     */
    Hydra2IntervalTemporalOperator(size_t start);

    /**
     * Constructor defining nothing. Therefore, interval is defautl interval [0, inf].
     */
    Hydra2IntervalTemporalOperator() = default;
    /**
     * Constructor specifying dynamic interval.
     *
     * @param dynamicInterval
     */
    Hydra2IntervalTemporalOperator(
        std::tuple<std::shared_ptr<PredicateMonitor>, std::shared_ptr<IntervalFunction>> dynamicInterval);

    /**
     * Constructor using interval expression.
     *
     * @param interval Pointer to interval expression.
     */
    Hydra2IntervalTemporalOperator(const std::shared_ptr<Interval> &interval);

    /**
     * Getter for start of interval.
     *
     * @return Start of interval.
     */
    size_t getStart() const override;

    /**
     * Getter for end of interval.
     *
     * @return End of interval.
     */
    size_t getEnd() const override;

    /**
     * Creates string representation of interval.
     *
     * @return String representation.
     */
    std::string to_string() override;

    void updateDynamicInterval(size_t stepNumber);

    void setTrace(const Trace& trace);

  private:
    HydraIntervalTriggers dynamicInterval; //**< Predicate and function trigger for dynamic interval. */
    bool dynamic;        //**< Boolean indicating whether interval is dynamic. */
};