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
  public:
    HydraMonitor(Formula formula);

    virtual Verdict step() = 0;

    const Formula &getFormula() const;

    virtual void setTrace(const Trace &tr) = 0;

    virtual void reset() = 0;

  protected:
    Formula formula;
    std::map<std::string, size_t> parentHeads;
    std::vector<bool> verdictHistory;
    std::optional<std::string> id;
};
