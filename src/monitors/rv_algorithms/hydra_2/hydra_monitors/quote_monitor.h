#pragma once

#include "monitors/collections/value.h"
#include "monitors/utils/utils.h"
#include "quantifier_monitor.h"

/**
 * Hydra2 monitor representing quote monitor.
 */
class QuoteMonitor : public QuantifierMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param monitorId ID of monitor.
     * @param variables Variables used by quantifier.
     * @param domains Collection of domains.
     * @param minQuote Minimum quote.
     * @param maxQuote Maximum quote.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     * @param shortCircuiting Boolean indicating whether short circuiting should be used.
     */
    QuoteMonitor(Formula formula, std::shared_ptr<Hydra2Monitor> subMonitor, size_t monitorId,
                 std::map<std::string, std::string> variables, DomainCollection &domains, float minQuote,
                 float maxQuote, bool connectedEval, bool shortCircuiting);

    /**
     * Computes the ratio of true subverdicts and number of combinations.
     *
     * @param trueVerdicts Number of true subverdict
     * @return Ratio.
     */
    float computeRatio(size_t trueVerdicts);

    /**
     * Performs concrete step of quote monitor. The ratio of true subverdicts and number of combinations
     * is at least as high as the minimum quote and at most as high as the maximum quote.
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
     * @return String representation monitor.
     */
    std::string to_string() override;

  private:
    float minQuote; //**< Minimum quote. */
    float maxQuote; //**< Maximum quote. */
};
