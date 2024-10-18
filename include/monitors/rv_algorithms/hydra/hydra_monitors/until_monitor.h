#pragma once

#include "../../../expression_tree/atomic/constant.h"
#include "../../../utils/interval.h"
#include "constant_monitor.h"
#include "hydra_monitor.h"
#include <utility>

/**
 * Hydra monitor for representing until operator.
 */
class UntilMonitorHydra : public HydraMonitor {
  public:
    /**
     *
     * @param formula Formula to be monitored.
     * @param subMonitorF Monitor from left side of until operator.
     * @param subMonitorG Monitor from right side of until operator.
     * @param interval  Interval of operator.
     */
    UntilMonitorHydra(const Formula &formula, std::shared_ptr<HydraMonitor> subMonitorF,
                      std::shared_ptr<HydraMonitor> subMonitorG, IntervalTemporalOperator interval)
        : HydraMonitor(formula), subMonitorF(std::move(subMonitorF)), subMonitorG(std::move(subMonitorG)),
          subMonitorH(std::make_shared<ConstantMonitorHydra>(Formula(std::make_shared<Constant>(true)))),
          interval(interval){};

    /**
     * Performs concrete step of until-monitor.
     *
     * @return Verdict for monitoring step result.
     */
    Verdict concreteStep() override;

    /**
     * Setter for trace.
     *
     * @param trace Trace.
     */
    void setTrace(const std::shared_ptr<Trace> &tra) override;

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
    /**
     * Executes left and right monitors of until.
     */
    void readFG();

    /**
     * Checks whether end of trace is reached and sets variables accordingly.
     */
    void handleEof();

    std::shared_ptr<HydraMonitor> subMonitorF; //**< Monitor for specification on left side of until. */
    std::shared_ptr<HydraMonitor> subMonitorG; //**< Monitor for specification on right side of until. */
    std::shared_ptr<HydraMonitor>
        subMonitorH; //**< Monitor to extract time step and index from trace at position of until monitor
                     // head. Represented by Constant-True-Monitor. */
    std::optional<Verdict> subVerdictF;              //**< Verdict for monitor on left side of until. */
    std::optional<Verdict> subVerdictG;              //**< Verdict for monitor on right side of until. */
    std::optional<Verdict> subVerdictH;              //**< Monitor to extract time step and index from trace at position
                                                     // of until monitor head. Represented by Constant-True-Monitor. */
    IntervalTemporalOperator interval;               //**< Interval of operator. */
    HeadPosHydra positionMonitorF;                   //**< Head position of left monitor. */
    HeadPosHydra positionMonitorG;                   //**< Head position of right monitor. *
    HeadPosHydra positionMonitorH;                   //**< Head position of base time monitor. *
    std::optional<std::pair<size_t, int>> last_sat;  //**< Last time right side of until was satisfied. */
    std::optional<std::pair<size_t, int>> last_viol; //**< Last time left side of until was violated. */
};
