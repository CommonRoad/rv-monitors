#pragma once

#include <utility>

#include "../../../utils/interval.h"
#include "hydra_monitor.h"

/**
 * Hydra2 monitor for representing until operator.
 */
class UntilMonitor final : public Hydra2Monitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorLeft Monitor from left side of until operator.
     * @param subMonitorRight Monitor from right side of until operator.
     * @param interval  Interval of operator.
     * @param monitorId ID of monitor.
     */
    UntilMonitor(const Formula &formula, std::shared_ptr<Hydra2Monitor> subMonitorLeft,
                 std::shared_ptr<Hydra2Monitor> subMonitorRight, const IntervalTemporalOperator &interval,
                 const size_t monitorId)
        : Hydra2Monitor(formula, monitorId, subMonitorRight->getEvalCost() + subMonitorLeft->getEvalCost(),
                        computeSatisfactionProbability(subMonitorLeft, subMonitorRight, interval)),
          subMonitorLeft(std::move(subMonitorLeft)), subMonitorRight(std::move(subMonitorRight)), interval(interval) {}

    /**
     * Performs concrete step of until-monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step/position in trace.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Setter for trace.
     *
     * @param tra Trace.
     * @param domains List of domains.
     */
    void setTrace(const std::shared_ptr<Trace> &tra,
                  const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Resets monitor variables to default values.
     */
    void reset() override;

    /**
     * Calls sub-monitors to add domain value.
     *
     * @param vals Domain values which should be added.
     */
    void addDomainValues(const std::vector<DomainValue> &vals) override;

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
     * @return String representing monitor.
     */
    std::string to_string() override;

    /**
     * Extracts tree structure for visualization.
     *
     * @param tree Mapping of monitor ID to its logical representation and child nodes.
     */
    void to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) override;

  private:
    std::shared_ptr<Hydra2Monitor> subMonitorLeft;  //**< Monitor for specification on left side of until. */
    std::shared_ptr<Hydra2Monitor> subMonitorRight; //**< Monitor for specification on right side of until. */
    IntervalTemporalOperator interval;              //**< Interval of operator. */
    std::vector<DomainValue> domainValueArguments;  //**< Domain values. */
    std::vector<size_t> timeStepsTrace;             //**< Time steps of trace. */
    std::vector<signed char>
        untilUnbounded; //**< Cached verdicts of (left U[0, inf] right) per step: -1 unknown, 0 false, 1 true. */

    /**
     * Evaluates left side of until at a step. Steps at which a domain value is invalid evaluate to false.
     *
     * @param stepNumber Step of interest.
     * @return Verdict of left monitor.
     */
    bool left(size_t stepNumber);

    /**
     * Evaluates right side of until at a step. Steps at which a domain value is invalid evaluate to false.
     *
     * @param stepNumber Step of interest.
     * @return Verdict of right monitor.
     */
    bool right(size_t stepNumber);

    /**
     * Evaluates (left U[0, inf] right) at a step. Results are cached so that each step is evaluated at most once.
     *
     * @param stepNumber Step of interest.
     * @return Verdict of unbounded until.
     */
    bool evalUnbounded(size_t stepNumber);

    /**
     * Computes satisfaction probability of until monitor based on left and right monitors.
     *
     * @param left Left monitor of until operator.
     * @param right Right monitor of until operator.
     * @param iv Interval of operator.
     * @return Satisfaction probability of until operator.
     */
    [[nodiscard]] static double computeSatisfactionProbability(const std::shared_ptr<Hydra2Monitor> &left,
                                                               const std::shared_ptr<Hydra2Monitor> &right,
                                                               const IntervalTemporalOperator &iv);
};
