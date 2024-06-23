#pragma once

#include "../../../utils/interval.h"
#include "hydra_monitor.h"
#include <utility>

/**
 * Hydra2 monitor for representing since operator.
 */
class SinceMonitor : public Hydra2Monitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorLeft Monitor from left side of until operator.
     * @param subMonitorRight Monitor from right side of until operator.
     * @param interval  Interval of operator.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    SinceMonitor(const Formula &formula, std::shared_ptr<Hydra2Monitor> subMonitorF,
                 std::shared_ptr<Hydra2Monitor> subMonitorG, IntervalTemporalOperator interval, size_t monitorId,
                 bool connectedEval)
        : Hydra2Monitor(formula, monitorId, connectedEval,
                        (subMonitorG->getEvalCost() + subMonitorF->getEvalCost()) / 2,
                        (subMonitorG->getSatisfactionProbability() + subMonitorG->getSatisfactionProbability()) / 2),
          subMonitorLeft(std::move(subMonitorF)), subMonitorRight(std::move(subMonitorG)), interval(interval){};

    /**
     * Performs concrete step of since-monitor.
     *
     * @param parentId ID of parent monitor.
     * @param stepNumber Current step/position in trace.
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep(size_t parentId, size_t stepNumber) override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     * @param domains List of domains.
     */
    void setTrace(const Trace &tra, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

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
     * @return New shared_ptr of this monitor.
     */
    std::shared_ptr<Hydra2Monitor> clone() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

    /**
     * Sets step number of monitor and sub-monitors.
     *
     * @param stepNumber Desired step number.
     * @param parentId ID of parent monitor.
     */
    void setStepNumber(size_t stepNumber, size_t parentId) override;

    /**
     * Extracts tree structure for visualization.
     *
     * @param tree Mapping of monitor ID to its logical representation and child nodes.
     */
    void to_tree(std::map<size_t, std::tuple<std::string, std::vector<size_t>>> &tree) override;

  private:
    std::shared_ptr<Hydra2Monitor> subMonitorLeft;  //**< Monitor for specification on left side of since. */
    std::shared_ptr<Hydra2Monitor> subMonitorRight; //**< Monitor for specification on right side of since. */
    std::optional<Verdict> subVerdictLeft;          //**< Verdict for monitor on left side of since. */
    std::optional<Verdict> subVerdictRight;         //**< Verdict for monitor on right side of since. */
    IntervalTemporalOperator interval;              //**< Interval of operator. */
    HeadPosHydra2 headPositionMonitorLeft;          //**< Head position of left monitor. */
    HeadPosHydra2 headPositionMonitorRight;         //**< Head position of right monitor. *
    std::optional<HeadPosHydra2> lastSatisfaction;  //**< Last time right side of until was satisfied. */
    std::optional<HeadPosHydra2> lastViolation;     //**< Last time left side of until was violated. */
    size_t stepNumberRight{0}; // Custom step number for right monitor since left monitor is only called once
    bool endReached{false};    //**< Boolean indicating whether end of trace was reached. */
    std::vector<DomainValue> domainValueArguments; //**< Domain values. */
};
