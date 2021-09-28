//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#pragma once

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
    void visitMonitor(HydraMonitor &mon);

    std::shared_ptr<HydraMonitor> createMonitor(Formula &formula);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitAtomic(Atomic &op);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitConstant(Constant &op);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPreviously(Previously &op);

    [[nodiscard]] std::shared_ptr<HydraMonitor> visitPredicate(Predicate &op);
};
