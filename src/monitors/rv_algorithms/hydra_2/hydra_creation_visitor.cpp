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
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include "monitors/formula/formula.h"
#include <numeric>
#include <utility>

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::createMonitor(Formula &formula, size_t parentId,
                                                                    bool connectedEval) {
    auto newMonitor{formula.getExpr()->accept(*this, parentId, connectedEval)};
    newMonitor->setFormulaId(formula.getId());
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitAtomic(Atomic &top, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<AtomicMonitor>(Formula(std::make_shared<Atomic>(top)), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitConstant(Constant &top, size_t parentId,
                                                                    bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{
            std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(top)), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitPredicate(Predicate &top, size_t parentId,
                                                                     bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        double cost{0.0};
        double prob{1.0};
        for (const auto &pred : predicateCosts)
            if (std::get<0>(pred) == top.getName()) {
                cost = pred.second[0];
                prob = pred.second[1];
                break;
            }
        auto newMonitor{std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(top)), idx,
                                                           connectedEval, cost, prob)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitPreviously(Previously &top, size_t parentId,
                                                                      bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<PreviousMonitor>(
            Formula(std::make_shared<Previously>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getInterval()->getStart(), top.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitNext(NextStrong &top, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<NextMonitor>(
            Formula(std::make_shared<NextStrong>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getInterval()->getStart(), top.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitSince(Since &top, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<SinceMonitor>(
            Formula(std::make_shared<Since>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getRight()->accept(*this, idx, connectedEval), top.getInterval()->getStart(),
            top.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitUntil(Until &top, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<UntilMonitor>(
            Formula(std::make_shared<Until>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getRight()->accept(*this, idx, connectedEval), top.getInterval()->getStart(),
            top.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitOr(Or &top, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<Hydra2Monitor>> monitors;
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        for (const auto &expr : top.getExpressions())
            monitors.push_back(expr->accept(*this, idx, connectedEval));
        if (reordering)
            reorder(monitors, false);
        auto newMonitor{std::make_shared<OrMonitor>(Formula(std::make_shared<Or>(top)), monitors, idx, connectedEval,
                                                    shortCircuiting)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitAnd(And &top, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<Hydra2Monitor>> monitors;
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        for (const auto &expr : top.getExpressions())
            monitors.push_back(expr->accept(*this, idx, connectedEval));
        if (reordering)
            reorder(monitors, true);
        auto newMonitor{std::make_shared<AndMonitor>(Formula(std::make_shared<And>(top)), monitors, idx, connectedEval,
                                                     shortCircuiting)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitNot(Not &top, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<NegationMonitor>(
            Formula(std::make_shared<Not>(top)), top.getLeft()->accept(*this, idx, connectedEval), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

HydraCreationVisitor2::HydraCreationVisitor2(std::map<std::string, std::array<double, 2>> predicateCosts,
                                             bool reordering, bool shortCircuiting)
    : reordering(reordering), shortCircuiting(shortCircuiting), predicateCosts(std::move(predicateCosts)) {
    for (const auto &pred : this->predicateCosts)
        assert((pred.second[0] >= 0 and pred.second[0] <= 100 and pred.second[1] >= 0 and pred.second[1] <= 1) &&
               "Invalid predicate costs/probabilities Hydra2 Creation Visitor.");
}

void HydraCreationVisitor2::reorder(std::vector<std::shared_ptr<Hydra2Monitor>> &monitors, bool increasing) {
    std::vector<double> cost;
    std::vector<size_t> tmp(monitors.size());
    cost.reserve(monitors.size());

    std::iota(tmp.begin(), tmp.end(), 0);
    if (increasing) { // and-operator
        for (const auto &mon : monitors)
            cost.push_back(mon->getSatisfactionProbability() * 10 + static_cast<double>(mon->getEvalCost()));
    } else { // or-operator
        for (const auto &mon : monitors)
            cost.push_back((1 - mon->getSatisfactionProbability()) * 10 + static_cast<double>(mon->getEvalCost()));
    }
    sort(tmp.begin(), tmp.end(), [&](size_t idx1, size_t idx2) { return cost[idx1] < cost[idx2]; });
    auto tmpSubMonitors{monitors};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        monitors[idx] = tmpSubMonitors.at(tmp.at(idx));
}
