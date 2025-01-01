#pragma once

#include "event.h"
#include "monitors/collections/constant_collection.h"
#include "monitors/collections/constant_value.h"
#include "monitors/collections/function.h"
#include "monitors/utils/verdict.h"
#include <algorithm>
#include <utility>
#include <vector>

/**
 * Class representing a trace of events.
 */
class Trace {
    std::vector<Event> trace;
    std::vector<std::shared_ptr<IFunction<bool>>> functionCollection;
    ConstantCollection constantCollection;

  public:
    /**
     * Constructor.
     * @param trace
     * @param functionCollection Collection of functions.
     * @param constantCollection Collection of constants.
     */
    explicit Trace(std::vector<Event> trace, std::vector<std::shared_ptr<IFunction<bool>>> functionCollection = {},
                   ConstantCollection constantCollection = {})
        : trace(std::move(trace)), functionCollection(std::move(functionCollection)),
          constantCollection(std::move(constantCollection)) {}

    /**
     * Default constructor.
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
     * Checks whether trace is equidistant.
     *
     * @return Boolean indicating whether trace is equidistant.
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

    /**
     * Extracts verdict for given step and event name.
     *
     * @param step Step of interest.
     * @param name Name of event
     * @return Verdict.
     */
    Verdict verdictAtTimeStep(size_t step, const std::string &name, const std::string &domName = "") {
        if (domName.empty())
            return trace.at(step).getVerdict(name);
        else if (!trace.at(step).hasVerdict(domName)) {
            auto verdict{trace.at(step).getVerdict(name)};
            trace.at(step).getEventMapRef().insert(
                {domName, EventValue<bool>(verdict.value)}); // store computed events in case trace will be re-used
            return verdict;
        } else
            return trace.at(step).getVerdict(domName);
    }

    /**
     * Assigns domain values to predicate.
     *
     * @param step Step of interest.
     * @param exprName Name of predicate.
     * @param argValues Relevant domain values.
     */
    void updateEvent(size_t step, const std::string &exprName, std::vector<DomainValue> &argValues) {
        argValues.insert(argValues.begin(), DomainValue::makeTimeStep(trace.at(step).getTimeStep()));
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

    /**
     * Getter for constant collection.
     *
     * @return Constant collection (container for constants).
     */
    [[nodiscard]] ConstantCollection getConstantCollection() const { return constantCollection; }

    /**
     * Clean trace by removing all entries containing any of the provided tokens.
     *
     * @param keep Tokens representing predicates to remove.
     * @param initialTimeStep Time step from which to start cleaning.
     */
    void cleanTrace(const std::vector<std::string> &keep, size_t initialTimeStep) {
        for (auto &event : trace) {
            if (event.getTimeStep() < initialTimeStep)
                continue;
            event.clean(keep);
        }
    }

    /**
     * Removes everything before and after provided step numbers.
     * @param begin Begin of the trace.
     * @param end End of the trace.
     * @param extension Number of time steps to add.
     */
    void removeAndExtend(size_t begin, size_t end, size_t extension) {
        trace.erase(std::remove_if(trace.begin(), trace.end(),
                                   [begin, end](const Event &event) {
                                       return event.getTimeStep() < begin || event.getTimeStep() > end;
                                   }),
                    trace.end());
        for (size_t idx{end + 1}; idx < end + extension + 1; ++idx)
            trace.emplace_back(idx, idx == end + extension - 1);
    }
};
