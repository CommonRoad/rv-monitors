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
#include "spdlog/spdlog.h"

#include <numeric>
#include <utility>

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::createMonitor(const Formula &formula, const size_t parentId,
                                                                    const bool connectedEval) {
    auto newMonitor{formula.getExpr()->accept(*this, parentId, connectedEval)};
    newMonitor->setFormulaId(formula.getId());
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitAtomic(Atomic &top, const size_t parentId,
                                                                  const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<AtomicMonitor>(Formula(std::make_shared<Atomic>(top)), idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitConstant(Constant &top, const size_t parentId,
                                                                    const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<ConstantMonitor>(Formula(std::make_shared<Constant>(top)), idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitPredicate(Predicate &top, const size_t parentId,
                                                                     const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    double cost{0.0};
    double prob{1.0};
    for (const auto &pred : predicateCosts)
        if (std::get<0>(pred) == top.getName()) {
            cost = pred.second[0];
            prob = pred.second[1];
            break;
        }
    auto newMonitor{std::make_shared<PredicateMonitor>(Formula(std::make_shared<Predicate>(top)), idx, cost, prob)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitPreviously(Previously &top, const size_t parentId,
                                                                      const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto opInterval{IntervalTemporalOperator(top.getInterval())};
    auto newMonitor{std::make_shared<PreviousMonitor>(
        Formula(std::make_shared<Previously>(top)), top.getLeft()->accept(*this, idx, connectedEval), opInterval, idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitNext(NextStrong &top, const size_t parentId,
                                                                const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto opInterval{IntervalTemporalOperator(top.getInterval())};
    auto newMonitor{std::make_shared<NextMonitor>(Formula(std::make_shared<NextStrong>(top)),
                                                  top.getLeft()->accept(*this, idx, connectedEval), opInterval, idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitSince(Since &top, const size_t parentId,
                                                                 const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto opInterval{IntervalTemporalOperator(top.getInterval())};
    auto newMonitor{std::make_shared<SinceMonitor>(Formula(std::make_shared<Since>(top)),
                                                   top.getLeft()->accept(*this, idx, connectedEval),
                                                   top.getRight()->accept(*this, idx, connectedEval), opInterval, idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitUntil(Until &top, const size_t parentId,
                                                                 const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto opInterval{IntervalTemporalOperator(top.getInterval())};
    auto newMonitor{std::make_shared<UntilMonitor>(Formula(std::make_shared<Until>(top)),
                                                   top.getLeft()->accept(*this, idx, connectedEval),
                                                   top.getRight()->accept(*this, idx, connectedEval), opInterval, idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitOr(Or &top, const size_t parentId,
                                                              const bool connectedEval) {
    std::vector<std::shared_ptr<Hydra2Monitor>> monitors;
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    for (const auto &expr : top.getExpressions())
        monitors.push_back(expr->accept(*this, idx, connectedEval));
    if (reordering)
        reorder(monitors, false);
    auto newMonitor{std::make_shared<OrMonitor>(Formula(std::make_shared<Or>(top)), monitors, idx, shortCircuiting)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitAnd(And &top, const size_t parentId,
                                                               const bool connectedEval) {
    std::vector<std::shared_ptr<Hydra2Monitor>> monitors;
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    for (const auto &expr : top.getExpressions())
        monitors.push_back(expr->accept(*this, idx, connectedEval));
    if (reordering)
        reorder(monitors, true);
    auto newMonitor{std::make_shared<AndMonitor>(Formula(std::make_shared<And>(top)), monitors, idx, shortCircuiting)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitNot(Not &top, const size_t parentId,
                                                               const bool connectedEval) {
    if (connectedEval and baseline.find(top.to_string()) != baseline.end()) {
        baseline.at(top.to_string())->registerParent(parentId);
        return baseline.at(top.to_string());
    }
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<NegationMonitor>(Formula(std::make_shared<Not>(top)),
                                                      top.getLeft()->accept(*this, idx, connectedEval), idx)};
    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

HydraCreationVisitor2::HydraCreationVisitor2(std::map<std::string, std::array<double, 3>> predicateCosts,
                                             const bool reordering, const bool shortCircuiting,
                                             const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : reordering(reordering), shortCircuiting(shortCircuiting), predicateCosts(std::move(predicateCosts)),
      domains(domains) {
    for (const auto &[fst, snd] : this->predicateCosts)
        assert(snd[0] >= 0 and snd[0] <= 100 and snd[1] >= 0 and snd[1] <= 1 &&
               "Invalid predicate costs/probabilities Hydra2 Creation Visitor.");
}

void HydraCreationVisitor2::reorder(std::vector<std::shared_ptr<Hydra2Monitor>> &monitors,
                                    const bool increasing) const {
    std::vector<std::vector<double>> costCombined;
    std::vector<std::vector<std::shared_ptr<Hydra2Monitor>>> orderCombined;

    // iterate first over all monitors and collect available priorities
    std::set<double> priorities{};
    for (const auto &mon : monitors) {
        std::string str{mon->to_string()};
        str = mon->to_string().substr(0, str.find('('));
        if (dynamic_cast<PredicateMonitor *>(mon.get())) {
            if (predicateCosts.find(str) != predicateCosts.end())
                priorities.insert(predicateCosts.at(str).at(2));
            else
                priorities.insert(0);
        } else
            priorities.insert(0);
    }
    priorities.insert({0}); // no predicate costs specified; we need at least 0 priority
    // prio == 0 represents lowes priority
    // fill missing priorities
    const double min{(*priorities.begin())};
    const double max{*priorities.rbegin()};
    for (double i{min}; i < max; ++i) {
        if (priorities.find(i) == priorities.end())
            priorities.insert(i);
    }

    // for each priority group, compute costs for corresponding monitors
    std::set<std::string> visitedMonitors;
    for (auto it = priorities.rbegin(); it != priorities.rend(); ++it) {
        const auto &prio = *it;
        std::vector<double> curCosts;
        std::vector<std::shared_ptr<Hydra2Monitor>> curOrder;
        for (const auto &mon : monitors) {
            if (!dynamic_cast<PredicateMonitor *>(mon.get()) or
                visitedMonitors.find(mon->to_string()) != visitedMonitors.end())
                continue;
            constexpr double eps{1e-9};
            double costTmp;
            if (increasing) // and-operator: stop on first FALSE -> optimal rank c / (1 - p)
                costTmp = mon->getEvalCost() / std::max(1.0 - mon->getSatisfactionProbability(), eps);
            else // or-operator: stop on first TRUE -> optimal rank c / p
                costTmp = mon->getEvalCost() / std::max(mon->getSatisfactionProbability(), eps);

            // add predicate monitor to correct priority group
            std::string str{mon->to_string()};
            str = mon->to_string().substr(0, str.find('('));

            if (predicateCosts.find(str) != predicateCosts.end() and predicateCosts.at(str).at(2) == prio) {
                curCosts.push_back(costTmp);
                curOrder.push_back(mon);
                visitedMonitors.insert(str);
            } else if (prio == 0 and predicateCosts.find(str) ==
                                         predicateCosts.end()) { // no priority specified or priority does not match
                curOrder.push_back(mon);
                curCosts.push_back(costTmp);
                visitedMonitors.insert(str);
            }
        }
        costCombined.push_back(curCosts);
        orderCombined.push_back(curOrder);
    }

    // take care of monitors that are no predicates
    std::vector<double> curCosts;
    bool otherMonitors{false};
    std::vector<std::shared_ptr<Hydra2Monitor>> curOrder;
    for (const auto &mon : monitors) {
        if (dynamic_cast<PredicateMonitor *>(mon.get()))
            continue;
        constexpr double eps{1e-9};
        double costTmp;
        if (increasing) // and-operator: stop on first FALSE -> optimal rank c / (1 - p)
            costTmp = mon->getEvalCost() / std::max(1.0 - mon->getSatisfactionProbability(), eps);
        else // or-operator: stop on first TRUE -> optimal rank c / p
            costTmp = mon->getEvalCost() / std::max(mon->getSatisfactionProbability(), eps);

        curOrder.push_back(mon);
        curCosts.push_back(costTmp);
        otherMonitors = true;
    }
    if (otherMonitors) {
        if (priorities.count(1) > 0) {
            costCombined.at(costCombined.size() - 2)
                .insert(costCombined.at(costCombined.size() - 2).end(), curCosts.begin(), curCosts.end());
            orderCombined.at(costCombined.size() - 2)
                .insert(orderCombined.at(costCombined.size() - 2).end(), curOrder.begin(), curOrder.end());
        } else {
            costCombined.insert(std::prev(costCombined.end()), curCosts);
            orderCombined.insert(std::prev(orderCombined.end()), curOrder);
        }
    }

    // assert to check that orderedCombined contains correct number of monitors
    assert(std::accumulate(orderCombined.begin(), orderCombined.end(), 0,
                           [](const size_t sum, const std::vector<std::shared_ptr<Hydra2Monitor>> &vec) {
                               return sum + vec.size();
                           }) == monitors.size() and
           "Reordering of monitors failed: number of monitors does not match the original vector size.");

    size_t costCounter{0};
    size_t monitorCounter{0};
    for (const auto &tmpSubMonitors : orderCombined) {
        auto cost{costCombined.at(costCounter)};
        std::vector<size_t> tmp(tmpSubMonitors.size());
        std::iota(tmp.begin(), tmp.end(), 0);
        sort(tmp.begin(), tmp.end(), [&](const size_t idx1, const size_t idx2) { return cost[idx1] < cost[idx2]; });
        for (size_t idx{0}; idx < tmpSubMonitors.size(); ++idx) {
            monitors[monitorCounter] = tmpSubMonitors.at(tmp.at(idx));
            ++monitorCounter;
        }
        ++costCounter;
    }
    assert(monitorCounter == monitors.size() and
           "Reordering of monitors failed: number of added monitors does not match the original vector size.");
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitUniversalQuantifier(UniversalQuantifier &top,
                                                                               const size_t parentId,
                                                                               const bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<UniversalMonitor>(Formula(std::make_shared<UniversalQuantifier>(top)),
                                                       top.getLeft()->accept(*this, idx, connectedEval), idx,
                                                       findDomain(top.getVariables()), shortCircuiting)};

    // Only top-level universal monitors store their violating domain values (Hydra2 enables this for the root
    // monitors), so that short-circuiting stays disabled only for them. Nested universal monitors short-circuit.
    newMonitor->setStoreVariableResults(false);

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitExistentialQuantifier(ExistentialQuantifier &top,
                                                                                 const size_t parentId,
                                                                                 const bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<ExistentialMonitor>(Formula(std::make_shared<ExistentialQuantifier>(top)),
                                                         top.getLeft()->accept(*this, idx, connectedEval), idx,
                                                         findDomain(top.getVariables()), shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitQuoteQuantifier(QuoteQuantifier &top, const size_t parentId,
                                                                           const bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<QuoteMonitor>(Formula(std::make_shared<QuoteQuantifier>(top)),
                                                   top.getLeft()->accept(*this, idx, connectedEval), idx,
                                                   findDomain(top.getVariables()), top.getQuoteInterval().getMin(),
                                                   top.getQuoteInterval().getMax(), shortCircuiting)};

    baseline.insert(std::make_pair<std::string, std::shared_ptr<Hydra2Monitor>>(top.to_string(), newMonitor));
    newMonitor->registerParent(parentId);
    return newMonitor;
}

std::shared_ptr<Hydra2Monitor> HydraCreationVisitor2::visitCountingQuantifier(CountingQuantifier &top,
                                                                              const size_t parentId,
                                                                              const bool connectedEval) {
    size_t idx{maxId++};
    auto newMonitor{std::make_shared<CountingMonitor>(
        Formula(std::make_shared<CountingQuantifier>(top)), top.getLeft()->accept(*this, idx, connectedEval), idx,
        findDomain(top.getVariables()), top.getOperator(), top.getNum(), shortCircuiting)};

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
