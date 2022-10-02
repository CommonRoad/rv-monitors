#pragma once

#include "lazy_function.h"
#include <sstream>
#include <unordered_map>

class Atomic;
class Predicate;

template <typename  T>
using event_map_t = std::unordered_map<std::string, LazyFunction<T>>;

/**
 * Event of a Trace.
 */
template <class T>
class Event {
  private:
    event_map_t<T> eventMap;
    size_t timeStep;
    bool terminal;

  public:
    /**
     * Constructor.
     * @param event Map from predicates to LazyBool.
     * @param terminal True if event is terminal, false other wise.
     */
    explicit Event(event_map_t<T> event, size_t time, bool terminal) noexcept
        : eventMap(std::move(event)), timeStep(time), terminal(terminal) {}

    Event(const Event &) = default;
    Event &operator=(const Event &) = default;
    Event(Event &&) noexcept = default;
    Event &operator=(Event &&) noexcept = default;

    /**
     * Getter for the value of an event given a string.
     *
     * @param name Name of event
     * @return Value of the event.
     */
    [[nodiscard]]
    T getValue(const std::string &name) const;

    /**
     * Getter for event map.
     * @return eventMap.
     */
    [[nodiscard]] event_map_t<T> getEventMap() const;

    /**
     * Getter for event map as reference.
     * @return Reference to event map.
     */
    [[nodiscard]] event_map_t<T> &getEventMapRef();

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
     * @param terminal
     */
    void setTerminal(bool terminal);

    /**
     * Create string of event.
     *
     * @return String representation.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Getter for time step.
     * @return Time step of event.
     */
    [[nodiscard]] size_t getTimeStep() const;
};
