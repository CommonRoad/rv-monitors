//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "hydra_creation_visitor.h"
#include "hydra_monitors/and_monitor.h"
#include "hydra_monitors/atomic_monitor.h"
#include "hydra_monitors/constant_monitor.h"
#include "hydra_monitors/negation_monitor.h"
#include "hydra_monitors/next_monitor.h"
#include "hydra_monitors/or_monitor.h"
#include "hydra_monitors/predicate_monitor.h"
#include "hydra_monitors/previous_monitor.h"
#include "hydra_monitors/since_monitor.h"
#include "hydra_monitors/until_monitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary//temporal/ltlf/until.h"
#include "monitors/expression_tree/binary//temporal/ptlf/since.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include "monitors/formula/formula.h"

std::shared_ptr<HydraMonitor> HydraCreationVisitor::createMonitor(Formula &formula, size_t parentId,
                                                                  bool connectedEval) {
    auto newMonitor{formula.getExpr()->accept(*this, parentId, connectedEval)};
    newMonitor->setFormulaId(formula.getId());
    return newMonitor;
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAtomic(Atomic &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<AtomicMonitor>(Formula(std::make_shared<Atomic>(op)), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitConstant(Constant &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(op)), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPredicate(Predicate &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{
            std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(op)), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPreviously(Previously &op, size_t parentId,
                                                                    bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<PreviousMonitor>(
            Formula(std::make_shared<Previously>(op)), op.getLeft()->accept(*this, id, connectedEval),
            op.getInterval()->getStart(), op.getInterval()->getEnd(), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNext(NextStrong &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<NextMonitor>(
            Formula(std::make_shared<NextStrong>(op)), op.getLeft()->accept(*this, id, connectedEval),
            op.getInterval()->getStart(), op.getInterval()->getEnd(), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitSince(Since &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<SinceMonitor>(
            Formula(std::make_shared<Since>(op)), op.getLeft()->accept(*this, id, connectedEval),
            op.getRight()->accept(*this, id, connectedEval), op.getInterval()->getStart(), op.getInterval()->getEnd(),
            id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitUntil(Until &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto tmpConstSubMonitor{
            std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(true)), maxId++, connectedEval)};
        tmpConstSubMonitor->registerParent(id);
        auto newMonitor{std::make_shared<UntilMonitor>(
            Formula(std::make_shared<Until>(op)), op.getLeft()->accept(*this, id, connectedEval),
            op.getRight()->accept(*this, id, connectedEval), tmpConstSubMonitor, op.getInterval()->getStart(),
            op.getInterval()->getEnd(), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitOr(Or &op, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        for (const auto &expr : op.getExpressions())
            monitors.push_back(expr->accept(*this, id, connectedEval));
        auto newMonitor{std::make_shared<OrMonitor>(Formula(std::make_shared<Or>(op)), monitors, id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAnd(And &op, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        for (const auto &expr : op.getExpressions())
            monitors.push_back(expr->accept(*this, id, connectedEval));
        auto newMonitor{std::make_shared<AndMonitor>(Formula(std::make_shared<And>(op)), monitors, id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNot(Not &op, size_t parentId, bool connectedEval) {
    if (baseline.find(op.to_string()) != baseline.end()) {
        baseline.at(op.to_string())->registerParent(parentId);
        return baseline.at(op.to_string());
    } else {
        size_t id{maxId++};
        auto newMonitor{std::make_shared<NegationMonitor>(
            Formula(std::make_shared<Not>(op)), op.getLeft()->accept(*this, id, connectedEval), id, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(op.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}
