#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/expression_tree/unary/first_order/existential_quantifier.h"
#include "monitors/expression_tree/unary/first_order/quote_quantifier.h"
#include "monitors/expression_tree/unary/first_order/universal_quantifier.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/and_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/counting_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/existential_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/negation_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/next_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/or_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/previous_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/quote_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/since_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/universal_monitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/until_monitor.h"
#include <cstring>
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
        auto opInterval{IntervalTemporalOperator(top.getInterval())};
        auto newMonitor{std::make_shared<PreviousMonitor>(Formula(std::make_shared<Previously>(top)),
                                                          top.getLeft()->accept(*this, idx, connectedEval), opInterval,
                                                          idx, connectedEval)};
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
        auto opInterval{IntervalTemporalOperator(top.getInterval())};
        auto newMonitor{std::make_shared<NextMonitor>(Formula(std::make_shared<NextStrong>(top)),
                                                      top.getLeft()->accept(*this, idx, connectedEval), opInterval, idx,
                                                      connectedEval)};
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
        auto opInterval{IntervalTemporalOperator(top.getInterval())};
        auto newMonitor{std::make_shared<SinceMonitor>(
            Formula(std::make_shared<Since>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getRight()->accept(*this, idx, connectedEval), opInterval, idx, connectedEval)};
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
        auto opInterval{IntervalTemporalOperator(top.getInterval())};
        auto newMonitor{std::make_shared<UntilMonitor>(
            Formula(std::make_shared<Until>(top)), top.getLeft()->accept(*this, idx, connectedEval),
            top.getRight()->accept(*this, idx, connectedEval), IntervalTemporalOperator(top.getInterval()), idx,
            connectedEval)};
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

HydraCreationVisitor2::HydraCreationVisitor2(std::map<std::string, std::array<double, 3>> predicateCosts,
                                             bool reordering, bool shortCircuiting,
                                             const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : reordering(reordering), shortCircuiting(shortCircuiting), predicateCosts(std::move(predicateCosts)),
      domains(domains) {
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
        for (const auto &mon : monitors) {
            double costTmp{mon->getSatisfactionProbability() * 10 + static_cast<double>(mon->getEvalCost())};
            std::string str{mon->to_string()};
            str = str.substr(0, str.find('('));
            if (predicateCosts.find(str) != predicateCosts.end())
                costTmp += predicateCosts.at(str).at(2);
            else
                costTmp += 100;
            cost.push_back(costTmp);
        }
    } else { // or-operator
        for (const auto &mon : monitors) {
            double costTmp{(1 - mon->getSatisfactionProbability()) * 10 + static_cast<double>(mon->getEvalCost())};
            std::string str{mon->to_string()};
            str = str.substr(0, str.find('('));
            if (predicateCosts.find(str) != predicateCosts.end())
                costTmp += predicateCosts.at(str).at(2);
            else
                costTmp += 2;
            cost.push_back(costTmp);
        }
    }
    sort(tmp.begin(), tmp.end(), [&](size_t idx1, size_t idx2) { return cost[idx1] < cost[idx2]; });
    auto tmpSubMonitors{monitors};
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        monitors[idx] = tmpSubMonitors.at(tmp.at(idx));
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitUniversalQuantifier(UniversalQuantifier &top,
                                                                               size_t parentId, bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<UniversalMonitor>(Formula(std::make_shared<UniversalQuantifier>(top)),
                                                       top.getLeft()->accept(*this, idx, connectedEval), idx,
                                                       findDomain(top.getVariables()), connectedEval, shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitExistentialQuantifier(ExistentialQuantifier &top,
                                                                                 size_t parentId, bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<ExistentialMonitor>(
        Formula(std::make_shared<ExistentialQuantifier>(top)), top.getLeft()->accept(*this, idx, connectedEval), idx,
        findDomain(top.getVariables()), connectedEval, shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitQuoteQuantifier(QuoteQuantifier &top, size_t parentId,
                                                                           bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<QuoteMonitor>(Formula(std::make_shared<QuoteQuantifier>(top)),
                                                   top.getLeft()->accept(*this, idx, connectedEval), idx,
                                                   findDomain(top.getVariables()), top.getQuoteInterval().getMin(),
                                                   top.getQuoteInterval().getMax(), connectedEval, shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitCountingQuantifier(CountingQuantifier &top, size_t parentId,
                                                                              bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<CountingMonitor>(
        Formula(std::make_shared<CountingQuantifier>(top)), top.getLeft()->accept(*this, idx, connectedEval), idx,
        findDomain(top.getVariables()), top.getOperator(), top.getNum(), connectedEval, shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::map<std::string, std::shared_ptr<DomainStorage>>
HydraCreationVisitor2::findDomain(const std::vector<Variable> &variables) const {
    std::map<std::string, std::shared_ptr<DomainStorage>> variableValues;
    for (const auto &var : variables) {
        for (const auto &dom : domains) {
            if (dom->getName() == var.getDomain().getName()) {
                variableValues[var.getName()] = dom;
                break;
            }
        }
        if (variableValues.count(var.getName()) != 1)
            variableValues[var.getName()] = {std::make_shared<DomainStorage>(var.getDomain().getName())};
    }
    return variableValues;
}
