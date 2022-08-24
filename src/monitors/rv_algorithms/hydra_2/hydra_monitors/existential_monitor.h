#ifndef RVMONITORS_EXISTENTIAL_MONITOR_H
#define RVMONITORS_EXISTENTIAL_MONITOR_H

#include "quantifier_monitor.h"
#include "monitors/utils/utils.h"
#include "monitors/collections/value.h"

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
    ExistentialMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                       std::map<std::string, std::string> variables, DomainCollection &domains,
                       bool connectedEval, bool shortCircuiting);

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
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

    /**
     * Computes normalized cost over provided monitor.
     *
     * @param monitor Monitor which should be used for existential evaluation cost computation.
     * @return Normalized evaluation cost of existential monitor in interval [0, 100].
     */
    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor);

    /**
     * Computes satisfaction probability over provided monitor.
     *
     * @param monitor Monitor which should be used for existential satisfaction probability computation.
     * @return Satisfaction probability of existential monitor in interval [0, 1].
     */
    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor);

};

#endif // RVMONITORS_EXISTENTIAL_MONITOR_H
