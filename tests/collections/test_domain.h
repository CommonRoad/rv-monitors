//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2023 - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/collections/domain.h"
#include <gtest/gtest.h>

class TestDomain : public testing::Test {
  protected:
    DomainStorage dom1;
    DomainStorage dom2;
    DomainStorage dom3;

  private:
    void SetUp() override;
};
