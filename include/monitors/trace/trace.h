#pragma once

#include "event.h"
#include "monitors/collections/constant_collection.h"
#include "monitors/collections/constant_value.h"
#include "monitors/collections/function.h"
#include "monitors/utils/verdict.h"
#include <algorithm>
#include <map>
#include <utility>
#include <vector>

/**
 * Class representing a trace of events.
 */
class Trace {
  private:
    std::vector<Event> trace;
    std::vector<std::shared_ptr<IFunction<bool>>> functionCollection;
    ConstantCollection constantCollection;

  public:
    /**
     * Constructor.
     * @param trace
     */
    explicit Trace(std::vector<Event> trace, std::vector<std::shared_ptr<IFunction<bool>>> functionCollection = {},
                   ConstantCollection constantCollection = {})
        : trace(std::move(trace)), functionCollection(std::move(functionCollection)),
          constantCollection(std::move(constantCollection)) {}

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

    /**
     * Extracts verdict for given step and event name.
     *
     * @param step Step of interest.
     * @param name Name of event
     * @return Verdict.
     */
    Verdict verdictAtTimeStep(size_t step, const std::string &name) { return trace.at(step).getVerdict(name); }

    /**
     * Assigns domain values to predicate.
     *
     * @param step Step of interest.
     * @param exprName Name of predicate.
     * @param argValues Relevant domain values.
     */
    void updateEvent(size_t step, const std::string &exprName, std::vector<DomainValue> &argValues) {
        argValues.insert(argValues.begin(),
                         DomainValue("time_step", trace.at(step).getTimeStep(), true,
                                     [](std::any val) { return std::to_string(std::any_cast<int>(val)); }));
        for (size_t idx = 0; idx < argValues.size(); ++idx) {
            if (constantCollection.contains(argValues[idx].getVariable()))
                argValues[idx] = DomainValue(argValues[idx].getVariable(),
                                             constantCollection.getConstant(argValues[idx].getVariable()).getValue(),
                                             true, [](std::any val) { return std::any_cast<std::string>(val); });
        }
        for (const auto &func : functionCollection)
            if (exprName.substr(0, exprName.find('(')) == func->getName().substr(0, func->getName().find('('))) {
                trace.at(step).addEventValueFunction(exprName, EventValue<bool>(func->getFunction(argValues)));
                break;
            }
    }

    /**
     * Add false events to trace at step.
     *
     * @param step Step of interest.
     * @param eventName Name of event which should be updated.
     */
    void updateEvent(size_t step, const std::string &eventName) {
        if (trace.at(step).getEventMap().find(eventName) == trace.at(step).getEventMap().end())
            trace.at(step).addEventValueFunction(eventName, EventValue<bool>(false));
    }
};
