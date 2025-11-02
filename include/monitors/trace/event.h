#pragma once

#include "monitors/trace/event_value.h"
#include "monitors/utils/verdict.h"
#include <algorithm>
#include <unordered_map>

class Predicate;
class Atomic;
class DomainValue;

using event_map_t = std::unordered_map<std::string, EventValue<bool>>;

/**
 * Event of a trace.
 */
class Event {
    event_map_t eventMap;
    size_t timeStep;
    bool terminal;

  public:
    /**
     * Constructor.
     * @param event Map from predicates to LazyBool.
     * @param time Time step of event.
     * @param terminal True if event is terminal, false otherwise.
     */
    explicit Event(event_map_t event, const size_t time, const bool terminal) noexcept
        : eventMap(std::move(event)), timeStep(time), terminal(terminal) {}

    /**
     *
     * @param time Time step of event.
     * @param terminal True if event is terminal, false otherwise.
     */
    explicit Event(const size_t time, const bool terminal) noexcept : timeStep(time), terminal(terminal) {}

    Event(const Event &) = default;
    Event &operator=(const Event &) = default;
    Event(Event &&) = default;
    Event &operator=(Event &&) = default;

    /**
     * Getter for the truth value of an atomic.
     * @param atomic
     * @return Truth value of the atomic.
     */
    [[nodiscard]] bool getValue(const std::string &atomic) const;

    /**
     * Getter for event map.
     * @return eventMap.
     */
    [[nodiscard]] event_map_t getEventMap() const;

    /**
     * Getter for event map as reference.
     * @return Reference to event map.
     */
    [[nodiscard]] event_map_t &getEventMapRef();

    /**
     * Getter for the truth value of an atomic.
     * @param atomic
     * @return Truth value of the atomic.
     */
    [[nodiscard]] bool getValue(const Atomic &atomic) const;

    /**
     * Getter for the truth value of an predicate.
     * @param predicate
     * @return Truth value of the predicate.
     */
    [[nodiscard]] bool getValue(const Predicate &predicate) const;

    /**
     *
     * @return True if terminal, false otherwise.
     */
    [[nodiscard]] bool isTerminal() const;

    /**
     * Setter for terminal attribute.
     * @param isTerminal Boolean indicating whether event should be terminal or not.
     */
    void setTerminal(bool isTerminal);

    /**
     * Creates string representation of event.
     *
     * @return String representation.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Getter for time step.
     * @return Time step of event.
     */
    [[nodiscard]] size_t getTimeStep() const;

    /**
     * Getter for verdict.
     *
     * @param name Name of verdict of interest.
     * @return Verdict.
     */
    Verdict getVerdict(const std::string &name);

    /**
     * Add event value function to event map.
     *
     * @param name Name of event value.
     * @param func Function representing event (can also be atomic prop.).
     */
    void addEventValueFunction(const std::string &name, EventValue<bool> func);

    /**
     * Checks whether event has verdict of provided name.
     *
     * @param name Name of predicate/atomic proposition.
     * @return Boolean indicating whether name is stored in event map.
     */
    bool hasVerdict(const std::string &name) { return eventMap.find(name) != eventMap.end(); }

    /**
     * Clean event map by removing all entries containing any of the provided tokens.
     *
     * @param keep Tokens representing predicates to remove.
     */
    void clean(std::vector<std::string> keep) {
        std::vector<std::string> toDelete;
        for (const auto &key : eventMap)
            if (std::find_if(keep.begin(), keep.end(), [key](const std::string &token) {
                    return key.first.find(token) != std::string::npos;
                }) != keep.end())
                toDelete.push_back(key.first);
        for (const auto &key : toDelete)
            eventMap.erase(key);
    }
};
