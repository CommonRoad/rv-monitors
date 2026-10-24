#pragma once

#include "../../../utils/interval.h"
#include "hydra_monitor.h"
#include <utility>

/**
 * Hydra2 monitor representing (strong) previous operator.
 */
class PreviousMonitor final : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param subMonitor Child monitor.
     * @param interval  Interval of operator.
     * @param monitorId ID of monitor.
     */
    PreviousMonitor(const Formula &formula, std::shared_ptr<Hydra2Monitor> subMonitor,
                    const IntervalTemporalOperator &interval, const size_t monitorId)
        : Hydra2Monitor(formula, monitorId, subMonitor->getEvalCost(), subMonitor->getSatisfactionProbability()),
          subMonitor(std::move(subMonitor)), interval(interval) {}

    /**
     * Performs concrete step of previous operator monitor.
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
     * Calls sub-monitor to add domain value.
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

  protected:
    [[nodiscard]] std::vector<std::shared_ptr<Hydra2Monitor>> getChildren() const override { return {subMonitor}; }

    // the child is evaluated at the previous step
    [[nodiscard]] VerdictRetention childRetention(const VerdictRetention &own) const override {
        return {own.steps + 1, own.time};
    }

  private:
    std::shared_ptr<Hydra2Monitor> subMonitor; //**< Child monitor. */
    IntervalTemporalOperator interval;         //**< Interval of operator. */
    std::vector<size_t> timeStepsTrace;        //**< Time steps of trace. */
};
