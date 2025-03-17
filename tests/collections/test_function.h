//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2022 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/collections/function.h"
#include <gtest/gtest.h>

class TestFunction : public testing::Test {
  protected:
    std::function<bool(size_t, std::string, double)> testf;
    std::shared_ptr<Function<bool, size_t, std::string, double>> test_f;

  private:
    void SetUp() override;
};
