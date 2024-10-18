#pragma once

#include "monitors/expression_tree/interval.h"
#include <cstddef>

/**
 * Representation of interval for temporal operators.
 */
struct IntervalTemporalOperator {
  public:
    /**
     * Constructor with all attributes.
     *
     * @param start Start of interval.
     * @param end End of interval.
     * @param finite Boolean indicating whether interval is finite.
     * @param empty Boolean indicating whether interval is empty, i.e. infinite interval.
     */
    IntervalTemporalOperator(size_t start, size_t end, bool finite, bool empty);

    /**
     * Constructor defining only start and end. Therefore, interval is not empty and finite.
     *
     * @param start Start of interval.
     * @param end End of interval.
     */
    IntervalTemporalOperator(size_t start, size_t end);

    /**
     * Constructor defining only start. End is infinite.
     *
     * @param start Start of interval.
     */
    explicit IntervalTemporalOperator(size_t start);

    /**
     * Constructor defining nothing. Therefore, interval is defautl interval [0, inf].
     */
    IntervalTemporalOperator();

    /**
     * Constructor using interval expression.
     *
     * @param interval Pointer to interval expression.
     */
    explicit IntervalTemporalOperator(const std::shared_ptr<Interval> &interval);

    /**
     * Getter for start of interval.
     *
     * @return Start of interval.
     */
    [[nodiscard]] size_t getStart() const;

    /**
     * Getter for end of interval.
     *
     * @return End of interval.
     */
    [[nodiscard]] size_t getEnd() const;

    /**
     * Getter for finite indication.
     *
     * @return Boolean indicating whether interval is finite.
     */
    [[nodiscard]] bool isFinite() const;

    /**
     * Getter for empty indication.
     *
     * @return Boolean indicating whether interval is empty.
     */
    [[nodiscard]] bool isEmpty() const;

  private:
    size_t start;              //**< Start of interval.  */
    std::optional<size_t> end; //**< End of interval.   */
    bool finite;               //**< Boolean indicating whether interval is finite.  */
    bool empty;                //**< Boolean indicating whether interval is empty, i.e. infinite interval.  */
};
