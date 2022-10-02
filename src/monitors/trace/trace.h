#pragma once

#include "event.h"
#include <utility>
#include <vector>

/**
 * Class representing a trace of events.
 */
class Trace {
  private:
    std::vector<Event<bool>> booleanElements;
    std::vector<Event<std::array<size_t, 2>>> intervalElements;

  public:
    /**
     * Constructor.
     * @param Trace
     */
    Trace(std::vector<Event<bool>> trace) : booleanElements(std::move(trace)) {}

    /**
     * Constructor.
     * @param Trace
     */
    Trace(std::vector<Event<bool>> booleanElements, std::vector<Event<std::array<size_t, 2>>> intervalElements) : booleanElements(std::move(booleanElements)), intervalElements(std::move(intervalElements)) {}

    /**
     * Constructor.
     * @param Trace
     */
    Trace() = default;

    /**
     * Getter for an event.
     * @param index
     * @return Event at index.
     */
    [[nodiscard]] Event<bool> &getBooleanEvent(size_t index);

    /**
     * Getter for an event.
     * @param index
     * @return Event at index.
     */
    [[nodiscard]] Event<std::array<size_t, 2>> &getIntervalEvent(size_t index);

    /**
     * Getter for trace length.
     * @return Length of the trace.
     */
    [[nodiscard]] size_t getLength();

    /**
     * Getter for the boolean trace.
     * @return trace.
     */
    [[nodiscard]] const std::vector<Event<bool>> &getBooleanTrace() const;

    /**
     * Getter for the interval function trace.
     *
     * @return trace.
     */
    [[nodiscard]] const std::vector<Event<std::array<size_t, 2>>> &getIntervalTrace() const;

    /**
     * Getter for trace reference.
     * @return Reference to trace.
     */
    [[nodiscard]] std::vector<Event<bool>> &getTraceRef();

    /**
     * To_string().
     * @return String representation of the trace.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Sets the terminal flag according to last.
     * @param last If last is true then the last event is terminal.
     */
    void setTerminalEvent(bool last);

    /**
     * Checks whether all time steps are equidistant in trace.
     *
     * @return Boolean indicating whether time steps are equidistant.
     */
    bool isEquidistant();
};
