#ifndef RVMONITORS_COUNTING_MONITOR_H
#define RVMONITORS_COUNTING_MONITOR_H

#include "quantifier_monitor.h"
#include "monitors/utils/utils.h"
#include "monitors/collections/value.h"

class CountingMonitor : public QuantifierMonitor {
  public:
    /**
     * Constructor
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param variables Variables used by quantifier.
     * @param domains Collection of domains.
     * @param minProbability Minimum probability.
     * @param maxProbability Maximum probability.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    CountingMonitor(Formula formula, std::shared_ptr<HydraMonitor> subMonitor, size_t monitorId,
                    std::map<std::string, std::string> variables, DomainCollection &domains,
                    float minProbability, float maxProbability, bool connectedEval, bool shortCircuiting);

    /**
     * Computes the ratio of true subverdicts and number of combinations.
     *
     * @param trueVerdicts Number of true subverdict
     * @return Ratio.
     */
    float computeRatio(size_t trueVerdicts);

    /**
     * Performs concrete step of counting monitor. The ratio of true subverdicts and number of combinations
     * is at least as high as the minimum probability and at most as high as the maximum probability.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step of monitor.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representation monitor.
     */
    std::string to_string() override;

    /**
     * Computes normalized cost over provided monitor.
     *
     * @param monitor Monitor which should be used for counting evaluation cost computation.
     * @return Normalized evaluation cost of counting monitor in interval [0, 100].
     */
    static double computeCost(const std::shared_ptr<HydraMonitor> &monitor);

    /**
     * Computes satisfaction probability over provided monitor.
     *
     * @param monitor Monitor which should be used for counting satisfaction probability computation.
     * @return Satisfaction probability of counting monitor in interval [0, 1].
     */
    static double computeSatisfactionProbability(const std::shared_ptr<HydraMonitor> &monitor);

  private:
    float minProbability; //**< Minimum probability. */
    float maxProbability; //**< Maximum probability. */

};

#endif // RVMONITORS_COUNTING_MONITOR_H
