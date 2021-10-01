//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include <map>
#include <memory>

class HydraMonitor;

class Formula;

class Expression;

class Unary;

class Binary;

class nary_interface;

class Atomic;

class Constant;

class And;

class Or;

class Xor;

class Implication;

class Not;

class Future;

class Globally;

class NextStrong;

class NextWeak;

class Once;

class Previously;

class Release;

class Until;

class Since;

class Historically;

class Trace;

class EventOperator;

class ResolvingOperator;

class DerivingOperator;

class Equivalence;

class Predicate;

class HydraCreationVisitor {
  public:
    std::shared_ptr<HydraMonitor> createMonitor(Formula &formula, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAtomic(Atomic &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitConstant(Constant &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPreviously(Previously &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPredicate(Predicate &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNext(NextStrong &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitSince(Since &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitUntil(Until &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitOr(Or &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAnd(And &op, size_t parentId);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNot(Not &op, size_t parentId);

  private:
    static size_t maxId;
    static std::map<std::string, std::shared_ptr<HydraMonitor>> baseline;
};
