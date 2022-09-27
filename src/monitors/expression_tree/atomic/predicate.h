//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/expression_tree/expression.h"
#include <utility>
#include <vector>

class Predicate : public Expression {
  public:
    Predicate(std::string symbol, std::string na, std::vector<std::string> params)
        : Expression(std::move(symbol)), name(std::move(na)), parameters(std::move(params)) {}

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    const std::string &getName() const;

    const std::vector<std::string> &getParameters() const;

  private:
    std::string name;
    std::vector<std::string> parameters;
};
