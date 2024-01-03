#pragma once

#include "event.h"
#include "monitors/collections/function.h"
#include "monitors/utils/verdict.h"
#include <utility>
#include <vector>

/**
 * Class representing a trace of events.
 */
class Trace {
  private:
    std::vector<Event> trace;
    std::shared_ptr<IFunction<bool>> functionCollection;

  public:
    /**
     * Constructor.
     * @param trace
     */
    explicit Trace(std::vector<Event> trace, std::shared_ptr<IFunction<bool>> functionCollection = {})
        : trace(std::move(trace)), functionCollection(std::move(functionCollection)) {}

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

    bool isEquidistant() {
        size_t expTimeStamp{0};
        for (const Event &event : trace) {
            if (event.getTimeStep() != expTimeStamp)
                return false;
            expTimeStamp++;
        }
        return true;
    }

    Verdict verdictAtTimeStep(size_t step, const std::string &name) { return trace.at(step).getVerdict(name); }

    void updateEvent(size_t step, const std::string &exprName, std::vector<DomainValue> &argValues) {
        argValues.insert(argValues.begin(), DomainValue("time_step", trace.at(step).getTimeStep()));
        auto func{functionCollection->getFunction(argValues)};
        trace.at(step).addEventValueFunction(exprName, EventValue<bool>(func));
    }
};
