#pragma once

#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/utils/utils.h"
#include "quantifier_monitor.h"

/**
 * Hydra2 monitor representing counting monitor.
 */
class CountingMonitor : public QuantifierMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param variables Variables used by quantifier.
     * @param domains Collection of domains.
     * @param cop Counting operator type.
     * @param num Number of operator.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    CountingMonitor(Formula formula, const std::shared_ptr<Hydra2Monitor> &subMonitor, size_t monitorId,
                    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues,
                    CountingQuantifier::Operator cop, size_t num, bool connectedEval, bool shortCircuiting);

    /**
     * Performs concrete step of counting monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @param numberCombinations Number of combinations of variables.
     * @return Verdict for monitoring step result.
     */
    Verdict quantifierStep(size_t parentId, size_t stepNumber, size_t numberCombinations) override;

    /**
     * Clones this monitor.
     *
     * @param domValNames List of domain value names.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone(const std::vector<std::string> &domValNames) override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representation monitor.
     */
    std::string to_string() override;

  private:
    CountingQuantifier::Operator op; //**< Operator type. */
    size_t num;                      //**< Number of operator. */
};
