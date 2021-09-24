//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/rv_monitor.h"
#include "monitors/trace/trace.h"
#include <gtest/gtest.h>

class TestRVMonitors : public testing::Test {
  protected:
    std::unique_ptr<Trace> trace;
    std::unique_ptr<Formula> formula;
    bool executeRVMonitor(AlgorithmType algorithm);

  private:
    void SetUp() override;
};
