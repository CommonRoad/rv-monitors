#pragma once

#include "event.h"
#include <vector>

/**
 * Class representing a trace of events.
 */
class Trace {
  private:
    std::vector<Event> trace;

  public:
    /**
     * Constructor.
     * @param trace
     */
    explicit Trace(std::vector<Event> trace) : trace(std::move(trace)) {}

    /**
     * Constructor.
     * @param trace
     */
    Trace() = default;

    /**
     * Getter for an event.
     * @param index
     * @return Event at index.
     */
    [[nodiscard]] Event &getEvent(size_t index) { return trace.at(index); }

    /**
     * Getter for trace length.
     * @return Length of the trace.
     */
    [[nodiscard]] size_t getLength() { return trace.size(); }

    /**
     * Getter for the trace.
     * @return trace.
     */
    [[nodiscard]] const std::vector<Event> &getTrace() const { return trace; }

    /**
     * Getter for trace reference.
     * @return Reference to trace.
     */
    [[nodiscard]] std::vector<Event> &getTraceRef() { return trace; }

    /**
     * To_string().
     * @return String representation of the trace.
     */
    [[nodiscard]] std::string to_string() const {
        std::stringstream ret;
        for (const Event &event : trace) {
            ret << event.to_string() << std::endl;
        }
        return ret.str();
    }

    /**
     * Sets the terminal flag according to last.
     * @param last If last is true then the last event is terminal.
     */
    void setTerminalEvent(bool last) {
        if (last) {
            trace.front().setTerminal(false);
            trace.back().setTerminal(true);
        } else {
            trace.front().setTerminal(true);
            trace.back().setTerminal(false);
        }
    }

    /**
     * Checks whether predicate is included by trace.
     * @param proposition Predicate proposition.
     * @param index Index of trace.
     * @return Boolean indicating whether the predicate is included by trace.
     */
    bool predicateIncludedByTrace(const std::string &proposition, size_t index) {
        if (index < trace.size()) {
            Event &event = trace[index];
            return event.hasPredicate(proposition);
        } else {
            return false;
        }
    }

    /**
     * Checks whether the distance between two adjacent time stamps are always equal in the complete trace.
     *
     * @return Boolean indicating whether the trace is equidistant or not.
     */
    bool isEquidistant() {
        size_t expTimeStamp{0};
        for (const Event &event : trace) {
            if (event.getTimeStep() != expTimeStamp)
                return false;
            expTimeStamp++;
        }
        return true;
    }
};
