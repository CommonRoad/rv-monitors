#pragma once

#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/rv_monitor.h"
#include "monitors/trace/trace.h"
#include <gtest/gtest.h>

class TestRVMonitors : public testing::Test {
  protected:
    std::shared_ptr<Trace> trace;
    std::unique_ptr<Formula> formula;
    std::vector<Verdict> executeRVMonitor(AlgorithmType algorithm);

  private:
    void SetUp() override;
};
