//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once
#include "monitors/utils/utils.h"

struct HeadPos {
    size_t ts;
    int tp = -1;
    HeadPos() = default;
    HeadPos(size_t timeStep, int traceIdx) : ts(timeStep), tp(traceIdx){};

    void update(size_t new_ts) {
        ts = new_ts;
        tp++;
    }

    void reset() {
        ts = 0;
        tp = -1;
    }
};

class HydraMonitor {
  private:
    size_t getMaxTraceIdx();

  public:
    /**
     * Base constructor for hydra monitor.
     *
     * @param formula Formula to monitor.
     * @param monitorId ID of monitor.
     * @param connectedEval Boolean whether connected evaluation should be performed.
     * @param evalCost Evaluation cost assigned to monitor.
     * @param satisfactionProbability Satisfaction probability of monitor.
     */
    HydraMonitor(Formula formula, size_t monitorId, bool connectedEval = true, double evalCost = 0,
                 double satisfactionProbability = 1.0);

    /**
     * General step function of hydra monitor.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @param ret
     * @param repeat
     * @return Boolean verdict.
     */
    Verdict step(size_t parentId, size_t stepNumber, bool ret = false, size_t repeat = 0);

    /**
     * Monitor specific step function.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @return Boolean verdict.
     */
    virtual Verdict concreteStep(size_t parentId, size_t stepNumber) = 0;

    /**
     * Getter for formula.
     *
     * @return Formula object.
     */
    const Formula &getFormula() const;

    /**
     * Setter for trace.
     *
     * @param tra Trace object.
     */
    virtual void setTrace(const Trace &tra) = 0;

    /**
     * Resets monitor variables to default values.
     */
    virtual void reset();

    /**
     * Registers parent monitor.
     *
     * @param parentId ID of parent monitor.
     */
    void registerParent(size_t parentId);

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

    /**
     * Getter for evaluation cost.
     *
     * @return Evaluation cost of monitor in interval [0, 100].
     */
    double getEvalCost() const;

    /**
     * Getter for satisfaction probability.
     *
     * @return Satisfaction probability of monitor in interval [0, 1].
     */
    double getSatisfactionProbability() const;

    /**
     * Creates string representation of monitor.
     *
     * @return String representing monitor.
     */
    virtual std::string to_string() = 0;

    /**
     * Extracts trace index based on parent ID (relevant for connected evaluation where several trace indices exist)
     *
     * @param parentId Id of parent monitor.
     * @return Trace index.
     */
    size_t getTraceIdxByParentId(size_t parentId) const;

  protected:
    Formula formula;                     //**< Monitored formula. */
    std::string formulaId;               //**< Id of formula */
    size_t monitorId;                    //**< Id of monitor */
    std::vector<Verdict> verdictHistory; //**< History of verdicts belonging to monitor */
    std::map<size_t, size_t> traceIdx;   //**< Index of current trace element (even) */
    bool connectedEval;                  //**< Boolean indicating whether connected evaluation should be performed */
    double evalCost{0.0};                //**< Evaluation cost of monitor */
    double satisfactionProbability{1.0}; //**< Satisfaction probability of monitor */
};
