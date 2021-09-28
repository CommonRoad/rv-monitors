//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/trace/trace.h"
#include "monitors/utils/utils.h"
#include <gtest/gtest.h>

class TestHydra : public testing::Test {
  public:
    static void evalHydra(Trace &trace, const Formula &formula, const std::vector<Verdict> &expSolution);
};
