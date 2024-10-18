#pragma once

#include "monitors/rv_algorithms/hydra/hydra_monitors/hydra_monitor.h"
#include <memory>
#include <optional>
#include <utility>

/**
 * Hydra monitor representing Constant (true/false).
 */
class ConstantMonitorHydra : public HydraMonitor {
  public:
    /**
     * Constructor.
     *
     * @param formula Formula to be monitored.
     */
    ConstantMonitorHydra(Formula formula) : HydraMonitor(std::move(formula)){};

    /**
     * Performs concrete step of constant monitor. It evaluates the trace element of the concrete step. Returns false if
     * traceIdx >= traceLength.
     *
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep() override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     */
    void setTrace(const std::shared_ptr<Trace> &trace) override;

    /**
     * Resets monitor variables to default values.
     */
    void reset() override;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    std::string to_string() override;

  private:
    size_t traceIdx{0};                          //**< Index of current trace element. */
    std::optional<std::shared_ptr<Trace>> trace; //**< Trace which should be monitored. */
};
