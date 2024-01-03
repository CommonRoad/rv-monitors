//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <utility>

#include "monitors/expression_tree/atomic/predicate.h"

class Domain : public Predicate {
  public:
    Domain(std::string symbol, std::string name, std::vector<std::string> parameters,
           std::vector<std::string> parameterVariables);

    /**
     * Returns whether domain is a function domain.
     *
     * @return Boolean indicating whether domain is a function domain.
     */
    bool isFunctionDomain() const;

    std::string to_string() const override;
};
