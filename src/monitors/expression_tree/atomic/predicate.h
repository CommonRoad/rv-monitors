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
    enum Argument {
        VARIABLE,
        PARAMETER,
        CONSTANT
    };

    Predicate(std::string symbol, std::string na, std::vector<std::pair<std::string, Argument>> args)
        : Expression(std::move(symbol)), name(std::move(na)), arguments(std::move(args)) {}

    Predicate(std::string symbol, std::string na, std::vector<std::string> args)
        : Expression(std::move(symbol)), name(std::move(na)) {
        for (const auto &arg : args)
            arguments.emplace_back(arg, PARAMETER);
    }

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId,
                                                       bool connectedEval) override;

    const std::string &getName() const;

    const std::vector<std::pair<std::string, Argument>> &getArguments() const;

  private:
    std::string name;
    std::vector<std::pair<std::string, Argument>> arguments;
};
