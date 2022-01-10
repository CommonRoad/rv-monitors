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

std::shared_ptr<HydraMonitor> HydraCreationVisitor::createMonitor(Formula &formula, size_t parentId,
                                                                  bool connectedEval) {
    auto newMonitor{formula.getExpr()->accept(*this, parentId, connectedEval)};
    newMonitor->setFormulaId(formula.getId());
    return newMonitor;
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAtomic(Atomic &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<AtomicMonitor>(Formula(std::make_shared<Atomic>(tlop)), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitConstant(Constant &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{
            std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(tlop)), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPredicate(Predicate &tlop, size_t parentId,
                                                                   bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        double cost;
        double prob;
        for (const auto &pred : predicateCosts)
            if (std::get<0>(pred) == tlop.getName()) {
                cost = pred.second[0];
                prob = pred.second[1];
                break;
            }
        auto newMonitor{std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(tlop)), idx,
                                                           connectedEval, cost, prob)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPreviously(Previously &tlop, size_t parentId,
                                                                    bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<PreviousMonitor>(
            Formula(std::make_shared<Previously>(tlop)), tlop.getLeft()->accept(*this, idx, connectedEval),
            tlop.getInterval()->getStart(), tlop.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNext(NextStrong &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<NextMonitor>(
            Formula(std::make_shared<NextStrong>(tlop)), tlop.getLeft()->accept(*this, idx, connectedEval),
            tlop.getInterval()->getStart(), tlop.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitSince(Since &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<SinceMonitor>(
            Formula(std::make_shared<Since>(tlop)), tlop.getLeft()->accept(*this, idx, connectedEval),
            tlop.getRight()->accept(*this, idx, connectedEval), tlop.getInterval()->getStart(),
            tlop.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitUntil(Until &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto tmpConstSubMonitor{
            std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(true)), maxId++, connectedEval)};
        tmpConstSubMonitor->registerParent(idx);
        auto newMonitor{std::make_shared<UntilMonitor>(
            Formula(std::make_shared<Until>(tlop)), tlop.getLeft()->accept(*this, idx, connectedEval),
            tlop.getRight()->accept(*this, idx, connectedEval), tmpConstSubMonitor, tlop.getInterval()->getStart(),
            tlop.getInterval()->getEnd(), idx, connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitOr(Or &tlop, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        for (const auto &expr : tlop.getExpressions())
            monitors.push_back(expr->accept(*this, idx, connectedEval));
        if (reordering)
            resort(monitors, false);
        auto newMonitor{std::make_shared<OrMonitor>(Formula(std::make_shared<Or>(tlop)), monitors, idx, connectedEval,
                                                    shortCircuiting)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAnd(And &tlop, size_t parentId, bool connectedEval) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        for (const auto &expr : tlop.getExpressions())
            monitors.push_back(expr->accept(*this, idx, connectedEval));
        if (reordering)
            resort(monitors, true);
        auto newMonitor{std::make_shared<AndMonitor>(Formula(std::make_shared<And>(tlop)), monitors, idx, connectedEval,
                                                     shortCircuiting)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNot(Not &tlop, size_t parentId, bool connectedEval) {
    if (connectedEval and baseline.find(tlop.to_string()) != baseline.end()) {
        baseline.at(tlop.to_string())->registerParent(parentId);
        return baseline.at(tlop.to_string());
    } else {
        size_t idx{maxId++};
        auto newMonitor{std::make_shared<NegationMonitor>(Formula(std::make_shared<Not>(tlop)),
                                                          tlop.getLeft()->accept(*this, idx, connectedEval), idx,
                                                          connectedEval)};
        baseline.insert(std::make_pair<std::string, std::shared_ptr<HydraMonitor>>(tlop.to_string(), newMonitor));
        newMonitor->registerParent(parentId);
        return newMonitor;
    }
}

HydraCreationVisitor::HydraCreationVisitor(const std::map<std::string, std::array<double, 2>> &predicateCosts,
                                           bool reordering, bool shortCircuiting)
    : reordering(reordering), shortCircuiting(shortCircuiting), predicateCosts(predicateCosts) {}

void HydraCreationVisitor::resort(std::vector<std::shared_ptr<HydraMonitor>> &monitors, bool increasing) {
    std::vector<double> cost;
    std::vector<size_t> tmp(monitors.size());
    cost.reserve(monitors.size());

    std::iota(tmp.begin(), tmp.end(), 0);
    if (increasing) {
        for (const auto &mon : monitors)
            cost.push_back(mon->getSatisfactionProbability() + static_cast<double>(mon->getEvalCost()));
    } else {
        for (const auto &mon : monitors)
            cost.push_back((1 - mon->getSatisfactionProbability()) + static_cast<double>(mon->getEvalCost()));
    }
    sort(tmp.begin(), tmp.end(), [&](size_t idx1, size_t idx2) { return cost[idx1] < cost[idx2]; });
    auto tmpSubMonitors{monitors};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        monitors[idx] = tmpSubMonitors.at(tmp.at(idx));
}