#pragma once

#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/expression.h"
#include "monitors/expression_tree/nary/nary_interface.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/monitor_interface.h"
#include "monitors/trace/trace.h"
#include <algorithm>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Dynamic Encapsulates functions needed by both DynamicFuture and DynamicPast for dynamic programming approach.
 */
class DynamicProgrammingLTL : public MonitorInterface {
  public:
    /**
     * Constructor for aDynamicProgrammingLTL.
     *
     * @param formula Formula that we want the DynamicProgrammingLTL for.
     * @param pastEvaluation Boolean indicating whether past evaluation has to be applied.
     */
    DynamicProgrammingLTL(const std::vector<Formula> &formulas, bool pastEvaluation = false);

    /**
     * Evaluation function for monitor algorithms.
     *
     * @param trace Trace which should be evaluated.
     * @param domains List of domains. Not used by DynamicProgrammingLTL.
     * @return List of verdicts for each time step of the trace. If the length of the vector is 1,
     * the runtime verification algorithm can only provide a global verdict for the complete trace.
     */
    std::map<std::string, std::vector<Verdict>>
    evaluate(Trace &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * to_string() function.
     *
     * @return String representation of the DynamicProgramming class.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Getter for pastEvaluation.
     *
     * @return Boolean indicating whether past evaluation is used.
     */
    [[nodiscard]] bool isPastEvaluation() const;

  private:
    bool pastEvaluation; //**< Boolean indicating whether past-based evaluation should be applied. */

    /**
     * Initializes vector pre/next with values according to eventMap.
     * @param idx Index of subformula in initVector.
     * @param initVector Vector where initial values are written to.
     * @param eventMap Map of events with truth values of all predicates.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     */
    void initialize(size_t idx, std::vector<bool> &initVector, event_map_t &eventMap,
                    const std::vector<std::shared_ptr<Expression>> &subformulas);

    /**
     * Updates newVector according to the values in oldVector.
     * @param idx Index of subformula vectors.
     * @param updateVector Subformula values of current event.
     * @param oldVector Subformula values of previous event.
     * @param eventMap Map of events with truth values of all predicates.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     */
    void update(size_t idx, std::vector<bool> &updateVector, std::vector<bool> oldVector, event_map_t &eventMap,
                const std::vector<std::shared_ptr<Expression>> &subformulas);

    /**
     * Computes truth value of an expression of type Implication or Nary.
     * @tparam F Expects two bool and return one bool.
     * @param idx Current index in vector that is evaluated.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     * @param values Evaluation vector.
     * @param function Function for fold.
     * @return Value of expression at index j in values vector.
     */
    template <class F>
    [[nodiscard]] bool applyBooleanOperation(size_t idx, const std::vector<std::shared_ptr<Expression>> &subformulas,
                                             const std::vector<bool> &values, F function) {
        // Implication
        if (auto impl = std::dynamic_pointer_cast<Implication>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), impl->getLeft());
            auto indexLeft = std::distance(subformulas.begin(), itr);
            itr = std::find(subformulas.begin(), subformulas.end(), impl->getRight());
            auto indexRight = std::distance(subformulas.begin(), itr);
            return function(values[indexLeft], values[indexRight]);
        }

        // And/Or/Xor
        // Iterate over set and apply function
        if (auto nary = std::dynamic_pointer_cast<NaryInterface>(subformulas[idx])) {
            bool res;
            for (auto exprItr = nary->begin(); exprItr != nary->end(); ++exprItr) {
                auto itr = std::find(subformulas.begin(), subformulas.end(), *exprItr);
                auto index = static_cast<size_t>(std::distance(subformulas.begin(), itr));
                if (exprItr == nary->begin()) {
                    res = values[index];
                } else {
                    res = function(res, values[index]);
                }
            }
            return res;
        }

        throw std::runtime_error("Dynamic::applyBooleanOperation() subformulas[idx] has to be Nary or Implication.");
    }

    /**
     * Initializes the vector pre with truth values holding in the first event of the trace.
     * @param event First event in trace.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     * @return Vector pre initialized.
     */
    [[nodiscard]] std::vector<bool> initializePre(Event &event,
                                                  const std::vector<std::shared_ptr<Expression>> &subformulas);

    /**
     * Updates the vector now using vector pre.
     * @param event Currently viewed event.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     * @param pre Vector from previous event.
     * @return Updated vector now.
     */
    [[nodiscard]] std::vector<bool> updateNowPast(Event &event,
                                                  const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                  const std::vector<bool> &pre);

    /**
     * Checks if a trace satisfies a formula by traversing it forwards.
     * @param formula pLTLf formula to be checked.
     * @param trace Event trace.
     * @return True if formula satisfied, false otherwise.
     */
    [[nodiscard]] bool checkTraceDynamicallyPast(Trace &trace);

    /**
     * Initializes the vector next with truth values holding in the last event of the trace.
     * @param event Last event in trace.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     * @return Vector next initialized.
     */
    [[nodiscard]] std::vector<bool> initializeNext(Event &event,
                                                   const std::vector<std::shared_ptr<Expression>> &subformulas);

    /**
     * Updates the vector now using vector next.
     * @param event Currently viewed event.
     * @param subformulas Vector of all subformulas of the formula to be verified.
     * @param next Vector from previous event.
     * @return Updated vector now.
     */
    [[nodiscard]] std::vector<bool> updateNowFuture(Event &event,
                                                    const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                    const std::vector<bool> &next);

    /**
     * Checks if a trace satisfies a formula by traversing it backwards.
     * @param formula LTLf formula to be checked.
     * @param trace Event trace.
     * @return True if formula satisfied, false otherwise.
     */
    [[nodiscard]] bool checkTraceDynamicallyFuture(Trace &trace);
};
