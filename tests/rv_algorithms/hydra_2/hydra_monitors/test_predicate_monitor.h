//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//

#pragma once
#include "monitors/trace/trace.h"
#include <gtest/gtest.h>

extern bool a(size_t dummy);
extern Trace createTrace();

class TestPredicateMonitor : public testing::Test {};
