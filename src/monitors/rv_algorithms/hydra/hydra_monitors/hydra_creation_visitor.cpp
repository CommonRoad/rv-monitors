//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include "monitors/formula/formula.h"
#include "predicate_monitor.h"
#include "previous_monitor.h"

std::shared_ptr<HydraMonitor> HydraCreationVisitor::createMonitor(Formula &formula) {
    return formula.getExpr()->accept(*this);
}

void HydraCreationVisitor::visitMonitor(HydraMonitor &mon) {
    // Unary
  //  if (auto *unary = dynamic_cast<Unary *>(mon.getSubMonitor()->get()->getFormula().getExpr())) {
  //      unary->setLeft(unary->getLeft()->accept(*this));
  //  }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAtomic(Atomic &op) {
    return std::make_shared<PredicateMonitor>(Formula(std::make_shared<Atomic>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitConstant(Constant &op) {
    return std::make_shared<PredicateMonitor>(Formula(std::make_shared<Constant>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPredicate(Predicate &op) {
    return std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(op)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPreviously(Previously &op) {
    return std::make_shared<PreviousMonitor>(Formula(std::make_shared<Previously>(op)), op.getLeft()->accept(*this),
                                             op.getInterval()->getStart(), op.getInterval()->getEnd());
}
