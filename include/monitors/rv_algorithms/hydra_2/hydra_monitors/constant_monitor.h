#pragma once

#include "hydra_monitor.h"
#include <memory>
#include <optional>
#include <utility>

/**
 * Hydra2 monitor representing Constant (true/false).
 */
class ConstantMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    ConstantMonitor(Formula formula, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(std::move(formula), monitorId, connectedEval){};

    /**
     * Performs concrete step of constant monitor. It evaluates the trace element of the concrete step. Returns false if
     * stepNumber >= traceLength.
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
    void setTrace(const Trace &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Adds domain value in other monitors. Not relevant for constant monitor.
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

  private:
    Trace trace; //**< Trace which should be monitored. */
};
