//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include <gtest/gtest.h>

class TestHydra : public testing::Test {
  public:
    static void executeHydraEval(Trace &trace, const std::vector<Formula> &formulas,
                                 const std::vector<std::vector<Verdict>> &expSolution);
    static void hydraEval(Trace &trace, const std::vector<Formula> &formulas,
                          const std::vector<std::vector<Verdict>> &expSolution);
    static void evalHydra(const std::vector<Formula> &formulas, const std::vector<std::vector<Verdict>> &expSolution,
                          Hydra &monitor, Trace &trac, const std::string &evaluation);
};
