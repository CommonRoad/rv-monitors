//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once
#include "deque"
#include "monitors/utils/utils.h"

/**
 * Representation of Hydra2 monitor head position consisting of time step and trace position.
 */
struct HeadPosHydra2 {
    size_t timeStep;   //**< Time step of monitor head position. */
    int traceIdx = -1; //**< Trace index of monitor head position. Initial invalid index. */

    /**
     * Default constructor.
     */
    HeadPosHydra2() = default;

    /**
     * Constructor of head position.
     *
     * @param timeStep Time step of monitor head position.
     * @param traceIdx Trace index of monitor head position.
     */
    HeadPosHydra2(size_t timeStep, int traceIdx) : timeStep(timeStep), traceIdx(traceIdx){};

    /**
     * Sets new time step and increments index by one.
     *
     * @param new_ts New time step.
     */
    void update(size_t new_ts) {
        timeStep = new_ts;
        traceIdx++;
    }

    /**
     * Reset head to default values.
     */
    void reset() {
        timeStep = 0;
        traceIdx = -1;
    }
};

/**
 * Hydra2 monitor interface.
 */
class Hydra2Monitor {
  private:
    /**
     * Extracts maximum trace idx of monitor given all parent monitors (required for connected evaluation).
     *
     * @return Trace index.
     */
    size_t getMaxTraceIdx();

    /**
     * Extracts minimum trace idx of monitor given all parent monitors (required for connected evaluation).
     *
     * @return Trace index.
     */
    size_t getMinTraceIdx();

    /**
     * Evaluates whether another parent monitor has the same trace index.
     *
     * @param idx Trace index which should be used for comparison.
     * @return Boolean indicating whether another parent monitor is at the same trace index.
     */
    bool traceIdxUnique(size_t idx);

    /**
     * Decrements trace indices for all parent monitors except for the provided ID.
     *
     * @param parentID Parent monitor ID where index should not be decremented.
     */
    void decrementTraceIdx(size_t parentID);

    /**
     * Executes monitor until desired step number is reached.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Last step number which should be performed.
     * @return Verdict of desired step number.
     */
    Verdict simulateVerdict(size_t parentId, size_t stepNumber);

    /**
     * Stores computed verdict of executed step.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @param verdict Verdict which should be stored.
     * @return Boolean verdict.
     */
    Verdict storeVerdict(size_t parentId, size_t stepNumber, Verdict verdict);

    /**
     * Loads already computed verdict for step.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @return Boolean verdict.
     */
    Verdict loadVerdict(size_t parentId, size_t stepNumber);

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
    Hydra2Monitor(Formula formula, size_t monitorId, bool connectedEval = true, double evalCost = 0,
                  double satisfactionProbability = 1.0);

    /**
     * General step function of hydra monitor.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @param singleStep Boolean indicating whether only one step for provided step number should be executed.
     * @return Boolean verdict.
     */
    Verdict step(size_t parentId, size_t stepNumber, bool singleStep = false);

    /**
     * Monitor specific step function.
     *
     * @param parentId Id of parent monitor.
     * @param stepNumber Number of step which should be performed.
     * @return Boolean verdict.
     */
    virtual Verdict concreteStep(size_t parentId, size_t stepNumber) = 0;

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
    void registerParent(size_t parentId, OperatorType parentOperatorType, std::optional<IntervalTemporalOperator> intv);

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

  protected:
    Formula formula;                     //**< Monitored formula. */
    std::string formulaId;               //**< Id of formula */
    size_t monitorId;                    //**< Id of monitor */
    std::deque<Verdict> verdictHistory;  //**< History of verdicts belonging to monitor */
    std::map<size_t, size_t> traceIdx;   //**< Index of current trace element (even) */
    bool connectedEval;                  //**< Boolean indicating whether connected evaluation should be performed */
    double evalCost{0.0};                //**< Evaluation cost of monitor */
    double satisfactionProbability{1.0}; //**< Satisfaction probability of monitor */
    std::map<size_t, OperatorType> parentType;                                //**< Type of parent monitor */
    std::map<size_t, std::optional<IntervalTemporalOperator>> parentInterval; //**< Interval of parent monitor */
    std::array<size_t, 2> minStepNumber; //**< Storage for minimum step number for any parent monitor with ID of
                                         // corresponding parent monitor. */
    size_t verdictHistoryPopCounter{0}; //**< Counter storing how often pop operation was applied on verdict history. */
};
