#pragma once

#include "monitors/collections/value.h"
#include "monitors/utils/utils.h"
#include "quantifier_monitor.h"

/**
 * Hydra2 monitor representing existential monitor.
 */
class ExistentialMonitor : public QuantifierMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param variables Variables used by quantifier.
     * @param domains Collection of domains.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    ExistentialMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                       std::map<std::string, std::string> variables, DomainCollection &domains, bool connectedEval,
                       bool shortCircuiting);

    /**
     * Performs concrete step of existential monitor. One of the subverdicts has to be true
     * so that the existential monitor returns also a true verdict for the current step number.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Clones this monitor.
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;
};
