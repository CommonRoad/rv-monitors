//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_creation_visitor.h"
#include "hydra_monitors/atomic_monitor.h"
#include "hydra_monitors/constant_monitor.h"
#include "hydra_monitors/next_monitor.h"
#include "hydra_monitors/predicate_monitor.h"
#include "hydra_monitors/previous_monitor.h"
#include "hydra_monitors/since_monitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary//temporal/ptlf/since.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include "monitors/formula/formula.h"

std::shared_ptr<HydraMonitor> HydraCreationVisitor::createMonitor(Formula &formula) {
    return formula.getExpr()->accept(*this);
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAtomic(Atomic &op) {
    return std::make_shared<AtomicMonitor>(Formula(std::make_shared<Atomic>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitConstant(Constant &op) {
    return std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPredicate(Predicate &op) {
    return std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPreviously(Previously &op) {
    return std::make_shared<PreviousMonitor>(Formula(std::make_shared<Previously>(op)), op.getLeft()->accept(*this),
                                             op.getInterval()->getStart(), op.getInterval()->getEnd());
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNext(NextStrong &op) {
    return std::make_shared<NextMonitor>(Formula(std::make_shared<NextStrong>(op)), op.getLeft()->accept(*this),
                                         op.getInterval()->getStart(), op.getInterval()->getEnd());
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitSince(Since &op) {
    return std::make_shared<SinceMonitor>(Formula(std::make_shared<Since>(op)), op.getLeft()->accept(*this),
                                          op.getRight()->accept(*this), op.getInterval()->getStart(),
                                          op.getInterval()->getEnd());
}