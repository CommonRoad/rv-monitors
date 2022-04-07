#pragma once

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/utils/lazy_bool.h"
#include <unordered_map>
#include <sstream>

using event_map_t = std::unordered_map<std::string, lazy_bool>;

/**
 * Event of a trace.
 */
class Event {
  private:
    event_map_t eventMap;
    size_t timeStep;
    bool terminal;

  public:
    /**
     * Constructor.
     * @param event Map from predicates to LazyBool.
     * @param terminal True if event is terminal, false other wise.
     */
    explicit Event(event_map_t event, size_t time, bool terminal) noexcept
        : eventMap(std::move(event)), timeStep(time), terminal(terminal) {}

    Event(const Event&) = default;
    Event& operator=(const Event&) = default;
    Event(Event&&) noexcept = default;
    Event& operator=(Event&&) noexcept = default;

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
     * Setter for eventMap.
     * @param eventMap
     */
    void setEventMap(const event_map_t &eventMap);

    /**
     * Setter for terminal attribute.
     * @param terminal
     */
    void setTerminal(bool terminal);

    [[nodiscard]] std::string to_string() const;

    /**
     * Getter for time step.
     * @return Time step of event.
     */
    [[nodiscard]] size_t getTimeStep() const;

    /**
     * Setter for time step.
     * @param timeStep time step to be set
     */
    void setTimeStep(size_t timeStep);
};
