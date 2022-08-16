//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

#include "monitors/collections/domain_collection.h"
#include "monitors/collections/function_collection.h"
#include <any>
#include <functional>
#include <map>
#include <memory>
#include <vector>

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

class UniversalQuantifier;

class ExistentialQuantifier;

class CountingQuantifier;

class SameFunc;

class HydraCreationVisitor {
  public:
    HydraCreationVisitor(std::map<std::string, std::array<double, 2>> predicate_costs,
                         DomainCollection &domains,
                         FunctionCollection &functions,
                         bool reordering = true, bool shortCircuiting = true);

    std::shared_ptr<HydraMonitor> createMonitor(Formula &formula, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAtomic(Atomic &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitConstant(Constant &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPreviously(Previously &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPredicate(Predicate &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNext(NextStrong &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitSince(Since &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitUntil(Until &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitOr(Or &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAnd(And &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitNot(Not &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitUniversalQuantifier(UniversalQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitExistentialQuantifier(ExistentialQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitCountingQuantifier(CountingQuantifier &tlop, size_t parentId, bool connectedEval);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitSameFunc(SameFunc &tlop, size_t parentId, bool connectedEval);

  private:
    size_t maxId{0};
    bool reordering;
    bool shortCircuiting;
    std::map<std::string, std::shared_ptr<HydraMonitor>> baseline;
    std::map<std::string, std::array<double, 2>> predicateCosts;
    DomainCollection domains;
    FunctionCollection functions;

    static void resort(std::vector<std::shared_ptr<HydraMonitor>> &monitors, bool increasing);
};
