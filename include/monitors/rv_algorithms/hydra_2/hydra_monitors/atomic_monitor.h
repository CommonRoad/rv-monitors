#pragma once

#include "hydra_monitor.h"

/**
 * Hydra2 monitor representing atomic proposition.
 */
class AtomicMonitor : public Hydra2Monitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be used.
     */
    AtomicMonitor(const Formula &formula, size_t monitorId, bool connectedEval)
        : Hydra2Monitor(formula, monitorId, connectedEval),
          name(std::dynamic_pointer_cast<Atomic>(this->formula.getExpr())->to_string()) {};

    /**
     * Performs concrete step of atomic monitor. It evaluates the trace element of the concrete step. Returns false if
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
    void setTrace(const std::shared_ptr<Trace> &trace,
                  const std::vector<std::shared_ptr<DomainStorage>> &domains = {}) override;

    /**
     * Adds domain value in other monitors. Not relevant for constant monitor.
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

  private:
    std::shared_ptr<Trace> trace; //**< Trace which should be monitored. */
    std::string name;             //**< Name of atomic value. */
    std::string domainValue;      //**< Name of domain if atomic is one otherwise empty string. */
};
