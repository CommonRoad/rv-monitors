#pragma once

#include "event.h"
#include "monitors/collections/constant_collection.h"
#include "monitors/collections/constant_value.h"
#include "monitors/collections/function.h"
#include "monitors/utils/verdict.h"
#include <algorithm>
#include <sstream>
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
    [[nodiscard]] Event &getEvent(const size_t index) { return trace.at(index); }

    /**
     * Getter for trace length.
     * @return Length of the trace.
     */
    [[nodiscard]] size_t getLength() const { return trace.size(); }

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
     * Converts trace to CSV format.
     *
     * @return String in csv style.
     */
    [[nodiscard]] std::string to_csv() const {
        if (trace.empty()) {
            return "";
        }

        // Collect all unique predicate names
        std::set<std::string> predicates;
        for (const auto &event : trace) {
            for (const auto &[fst, snd] : event.getEventMap()) {
                predicates.insert(fst);
            }
        }

        // Build the CSV content
        std::string csv = "Time Step";
        for (const auto &predicate : predicates) {
            csv += ";" + predicate;
        }
        csv += "\n";

        for (const auto &event : trace) {
            csv += std::to_string(event.getTimeStep());
            for (const auto &predicate : predicates) {
                const auto &eventMap = event.getEventMap();
                if (auto it = eventMap.find(predicate); it != eventMap.end())
                    csv += ";" + std::string(it->second.to_string());
                else
                    csv += ";Not initialized";
            }
            csv += "\n";
        }

        return csv;
    }

    /**
     * Inserts events to trace between start and end with given step size.
     *
     * @param start Start time step.
     * @param end End time step.
     * @param step Step size.
     */
    void insertEvents(const size_t start, const size_t end, const size_t step) {
        // Collect desired steps, skipping those already in the trace.
        // The trace is maintained in sorted order, so we walk both sequences together
        // to find missing steps in O(n + k) rather than O(n*k).
        std::vector<size_t> missingSteps;
        {
            size_t ti = 0;
            for (size_t t = start; t <= end; t += step) {
                while (ti < trace.size() && trace[ti].getTimeStep() < t)
                    ++ti;
                if (ti >= trace.size() || trace[ti].getTimeStep() != t)
                    missingSteps.push_back(t);
            }
        }

        if (missingSteps.empty())
            return;

        // Determine the new terminal step and clear the old terminal flag if needed.
        const size_t terminalStep =
            std::max(trace.empty() ? size_t{0} : trace.back().getTimeStep(), missingSteps.back());
        if (!trace.empty() && terminalStep > trace.back().getTimeStep())
            trace.back().setTerminal(false);

        // Merge existing events and new events in a single O(n + k) pass.
        std::vector<Event> merged;
        merged.reserve(trace.size() + missingSteps.size());
        size_t mi = 0;
        for (auto &evt : trace) {
            while (mi < missingSteps.size() && missingSteps[mi] < evt.getTimeStep()) {
                merged.emplace_back(missingSteps[mi], missingSteps[mi] == terminalStep);
                ++mi;
            }
            merged.push_back(std::move(evt));
        }
        while (mi < missingSteps.size()) {
            merged.emplace_back(missingSteps[mi], missingSteps[mi] == terminalStep);
            ++mi;
        }
        trace = std::move(merged);
    }

    /**
     * Sets the terminal flag according to last.
     * @param last If last is true then the last event is terminal.
     */
    void setTerminalEvent(const bool last) {
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
    [[nodiscard]] bool isEquidistant() const {
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
     * @param domName
     * @return Verdict.
     */
    Verdict verdictAtTimeStep(const size_t step, const std::string &name, const std::string &domName = "") {
        if (domName.empty())
            return trace.at(step).getVerdict(name);
        if (!trace.at(step).hasVerdict(domName)) {
            const auto verdict{trace.at(step).getVerdict(name)};
            trace.at(step).getEventMapRef().insert(
                {domName, EventValue(verdict.value)}); // store computed events in case trace will be re-used
            return verdict;
        }
        return trace.at(step).getVerdict(domName);
    }

    /**
     * Assigns domain values to predicate.
     *
     * @param step Step of interest.
     * @param exprName Name of predicate.
     * @param argValues Relevant domain values.
     */
    void updateEvent(const size_t step, const std::string &exprName, std::vector<DomainValue> &argValues) {
        argValues.insert(argValues.begin(), DomainValue::makeTimeStep(trace.at(step).getTimeStep()));
        for (size_t idx = 0; idx < argValues.size(); ++idx) {
            if (constantCollection.contains(argValues[idx].getVariable()))
                argValues[idx] = DomainValue(argValues[idx].getVariable(),
                                             constantCollection.getConstant(argValues[idx].getVariable()).getValue(),
                                             true, [](std::any val) { return std::any_cast<std::string>(val); });
        }
        for (const auto &func : functionCollection)
            if (exprName.substr(0, exprName.find('(')) == func->getName().substr(0, func->getName().find('('))) {
                trace.at(step).addEventValueFunction(exprName, EventValue(func->getFunction(argValues)));
                break;
            }
    }

    /**
     * Add false events to trace at step.
     *
     * @param step Step of interest.
     * @param eventName Name of event which should be updated.
     */
    void updateEvent(const size_t step, const std::string &eventName) {
        if (trace.at(step).getEventMap().find(eventName) == trace.at(step).getEventMap().end())
            trace.at(step).addEventValueFunction(eventName, EventValue(false));
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
    void cleanTrace(const std::vector<std::string> &keep, const size_t initialTimeStep) {
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
    void removeAndExtend(size_t begin, size_t end, const size_t extension) {
        trace.erase(std::remove_if(trace.begin(), trace.end(),
                                   [begin, end](const Event &event) {
                                       return event.getTimeStep() < begin || event.getTimeStep() > end;
                                   }),
                    trace.end());
        for (size_t idx{end + 1}; idx < end + extension + 1; ++idx)
            trace.emplace_back(idx, idx == end + extension - 1);
    }
};
