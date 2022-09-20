#pragma once

#include <iostream>
#include <memory>
#include <string>

/**
 * Class representing expression interval.
 */
class Interval {
  public:
    /**
     * Constructor.
     *
     * @param start Start of interval.
     * @param end End of interval.
     * @param finite Boolean indicating whether interval is finite.
     * @param empty Boolean indicating whether interval is not specified.
     */
    explicit Interval(int start = 0, int end = 10000, bool finite = false, bool empty = true);

    /**
     * Converts interval expression to string.
     *
     * @return String representation of interval.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Getter for start of interval.
     *
     * @return Start of interval.
     */
    [[nodiscard]] int getStart() const;

    /**
     * Getter for end of interval.
     *
     * @return End of interval.
     */
    [[nodiscard]] int getEnd() const;

    /**
     * Getter for finite.
     *
     * @return  Boolean indicating whether interval is finite.
     */
    [[nodiscard]] bool isFinite() const;

    /**
     * Computes length of interval.
     *
     * @return Length of interval.
     */
    [[nodiscard]] int getLength() const;

    /**
     * Creates a copy of the interval expression.
     *
     * @return Cloned interval.
     */
    [[nodiscard]] std::shared_ptr<Interval> clone() { return std::make_shared<Interval>(*this); }

    /**
     * Getter for empty.
     *
     * @return Boolean indicating whether no interval is specified for an operator.
     */
    bool isEmpty() const;

  private:
    int start;   //**< Start of time interval. */
    int end;     //**< End of time interval. */
    bool finite; //**< Boolean indicating whether interval is finite. */
    bool empty;  //**< Boolean indicating whether interval is specified for an operator. */
};
