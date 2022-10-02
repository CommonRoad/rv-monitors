#pragma once

#include <monitors/formula/formula.h>

/**
 * Class representing a multi-transition.
 */
class MultiTransition {
  private:
    // Outgoing transitions from state
    std::map<Formula, size_t> transitions;

  public:
    /**
     * Constructor.
     * @param transitions
     */
    explicit MultiTransition(std::map<Formula, size_t> transitions);

    /**
     * Getter for transitions.
     * @return transitions.
     */
    [[nodiscard]] std::map<Formula, size_t> &getTransitions();

    /**
     * Const getter for transitions.
     * @return transitions.
     */
    [[nodiscard]] const std::map<Formula, size_t> &getTransitions() const;

    /**
     * To_string().
     * @return String representation of the MT.
     */
    [[nodiscard]] std::string to_string() const;

    /**
     * Evaluates next state according to an event.
     * @param eventMap
     * @return Next state in MT-FSM*.
     */
    [[nodiscard]] size_t evaluate(event_map_t<bool> &eventMap) const;

    /**
     * Counts the frequency of all atomics in a MultiTransition.
     * @param multiTransition MultiTransition with formulas where atomic frequencies should be counted.
     * @return Map from string representation of each atomic to its frequency in the MultiTransition.
     */
    [[nodiscard]] std::map<std::string, size_t> getAtomicAndPredicateFrequencies() const;

    /**
     * Gives each atomic a weight w = (1/frequency) * cost. The higher the value the deeper the atomic is nested into
     * the tree.
     * @param multiTransition MultiTransition.
     * @return Multimap weight -> atomic. This way the atomics are already ordered.
     */
    std::multimap<double, std::shared_ptr<Atomic>> getAtomicAndPredicateWeight() const;

    /**
     * Helper function. Flips a map and turns it into a multimap.
     * @param weights Map of atomic -> weight.
     * @return Multimap weight -> atomic.
     */
    std::multimap<double, std::shared_ptr<Atomic>>
    createWeightMultimap(const std::map<std::shared_ptr<Atomic>, double> &weights) const;
};
