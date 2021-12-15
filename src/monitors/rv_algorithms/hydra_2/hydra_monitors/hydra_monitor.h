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
    bool eof{false};

    void update(size_t new_ts) {
        ts = new_ts;
        tp++;
    }

    void reset() {
        ts = 0;
        tp = -1;
        eof = false;
    }
};

class HydraMonitor {
  private:
    size_t getMaxTraceIdx();

  public:
    HydraMonitor(Formula formula, size_t id, bool connectedEval = true, double evalCost = 0,
                 double satisfactionProbability = 100.0);

    Verdict step(size_t parentId, size_t stepNumber, bool ret = false);

    virtual Verdict concreteStep(size_t parentId, size_t stepNumber) = 0;

    const Formula &getFormula() const;

    virtual void setTrace(const Trace &tra) = 0;

    virtual void reset();

    void registerParent(size_t parentId);

    const std::string &getFormulaId() const;

    void setFormulaId(const std::string &formula_id);

    double getEvalCost() const;

    double getSatisfactionProbability() const;

    virtual std::string to_string() = 0;

  protected:
    Formula formula;
    std::string formulaId;
    size_t monitorId;
    std::vector<Verdict> verdictHistory;
    std::map<size_t, size_t> traceIdx;
    bool connectedEval;
    double evalCost{0.0};
    double satisfactionProbability{100.0};
};
