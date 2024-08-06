//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once
#include "monitors/utils/utils.h"

struct HeadPosHydra {
    size_t ts;       //**< Time step of monitor head position. */
    int tp = -1;     //**< Trace index of monitor head position. Initial invalid index. */
    bool eof{false}; //**< Indicator whether end of trace was reached. */

    /**
     * Sets new time step and increments index by one.
     *
     * @param new_ts New time step.
     */
    void update(size_t new_ts) {
        ts = new_ts;
        tp++;
    }

    /**
     * Reset head to default values.
     */
    void reset() {
        ts = 0;
        tp = -1;
        eof = false;
    }
};

/**
 * Hydra monitor interface.
 */
class HydraMonitor {
  public:
    /**
     * Destructor.
     */
    virtual ~HydraMonitor() = default;

    /**
     * Base constructor for hydra monitor.
     *
     * @param formula Formula to monitor.
     */
    HydraMonitor(Formula formula);

    /**
     * Monitor specific step function.
     *
     * @return Boolean verdict.
     */
    virtual Verdict concreteStep() = 0;

    /**
     * Setter for trace.
     *
     * @param tra Trace object.
     */
    virtual void setTrace(const std::shared_ptr<Trace> &tra) = 0;

    /**
     * Resets monitor variables to default values.
     */
    virtual void reset() = 0;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    virtual std::string to_string() = 0;

    /**
     * Getter for ID of formula which is monitored.
     * @return
     */
    const std::string &getFormulaId() const;

    /**
     * Sets ID of formula which is monitored.
     *
     * @param formula_id Formula ID as string.
     */
    void setFormulaId(const std::string &formula_id);

  protected:
    Formula formula;       //**< Monitored formula. */
    std::string formulaId; //**< Id of formula */
};
