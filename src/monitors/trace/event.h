#pragma once

#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/utils/lazy_bool.h"
#include <map>
#include <sstream>

/**
 * Event of a trace.
 */
class Event {
  private:
    std::map<std::string, lazy_bool> eventMap;
    size_t timeStep;
    bool terminal;

  public:
    /**
     * Constructor.
     * @param event Map from predicates to LazyBool.
     * @param terminal True if event is terminal, false other wise.
     */
    explicit Event(std::map<std::string, lazy_bool> &event, size_t time, bool terminal)
        : eventMap(event), timeStep(time), terminal(terminal) {}

    /**
     * Getter for the truth value of an atomic.
     * @param atomic
     * @return Truth value of the atomic.
     */
    [[nodiscard]] bool getValue(const std::string &atomic);

    /**
     * Getter for event map.
     * @return eventMap.
     */
    [[nodiscard]] std::map<std::string, lazy_bool> getEventMap() const;

    /**
     * Getter for event map as reference.
     * @return Reference to event map.
     */
    [[nodiscard]] std::map<std::string, lazy_bool> &getEventMapRef();

    /**
     * Getter for the truth value of an atomic.
     * @param atomic
     * @return Truth value of the atomic.
     */
    [[nodiscard]] bool getValue(const Atomic &atomic);

    /**
     * Getter for the truth value of an predicate.
     * @param predicate
     * @return Truth value of the predicate.
     */
    [[nodiscard]] bool getValue(const Predicate &predicate);

    /**
     *
     * @return True if terminal, false otherwise.
     */
    [[nodiscard]] bool isTerminal() const;

    /**
     * Setter for eventMap.
     * @param eventMap
     */
    void setEventMap(const std::map<std::string, lazy_bool> &eventMap);

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
