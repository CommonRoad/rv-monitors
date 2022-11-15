#include "dynamic.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/next_weak.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include <monitors/utils/lazy_bool.h>

DynamicProgrammingLTL::DynamicProgrammingLTL(const std::vector<Formula> &formulas, bool pastEvaluation)
    : MonitorInterface(formulas, AlgorithmType::dynamic_programming_ltl), pastEvaluation(pastEvaluation) {}

void DynamicProgrammingLTL::initialize(size_t idx, std::vector<bool> &initVector, event_map_t &eventMap,
                                       const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[idx])) {
        initVector[idx] = eventMap.at(atomic->to_string());
    }
    // Predicate
    if (auto predicate = std::dynamic_pointer_cast<Predicate>(subformulas[idx])) {
        initVector[idx] = eventMap.at(predicate->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[idx])) {
        initVector[idx] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[idx])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        initVector[idx] = not initVector[indexLeft];
    }

    // And
    else if (std::dynamic_pointer_cast<And>(subformulas[idx])) {
        initVector[idx] = applyBooleanOperation(idx, subformulas, initVector,
                                                [&](const auto &left, const auto &right) { return left and right; });
    }
    // Or
    else if (std::dynamic_pointer_cast<Or>(subformulas[idx])) {
        initVector[idx] = applyBooleanOperation(idx, subformulas, initVector,
                                                [&](const auto &left, const auto &right) { return left or right; });
    }

    // Xor
    else if (std::dynamic_pointer_cast<Xor>(subformulas[idx])) {
        initVector[idx] = applyBooleanOperation(idx, subformulas, initVector,
                                                [&](const auto &left, const auto &right) { return left xor right; });
    }

    // Implication
    else if (std::dynamic_pointer_cast<Implication>(subformulas[idx])) {
        initVector[idx] = applyBooleanOperation(idx, subformulas, initVector,
                                                [&](const auto &left, const auto &right) { return not left or right; });
    }

    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(subformulas[idx])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), globally->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        initVector[idx] = initVector[indexLeft];
    }
}

void DynamicProgrammingLTL::update(size_t idx, std::vector<bool> &updateVector, std::vector<bool> oldVector,
                                   event_map_t &eventMap, const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[idx])) {
        updateVector[idx] = eventMap.at(atomic->to_string());
    }
    // Predicate
    if (auto predicate = std::dynamic_pointer_cast<Predicate>(subformulas[idx])) {
        updateVector[idx] = eventMap.at(predicate->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[idx])) {
        updateVector[idx] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[idx])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        updateVector[idx] = not updateVector[indexLeft];
    }

    // And
    else if (std::dynamic_pointer_cast<And>(subformulas[idx])) {
        updateVector[idx] = applyBooleanOperation(idx, subformulas, updateVector,
                                                  [&](const auto &left, const auto &right) { return left and right; });
    }
    // Or
    else if (std::dynamic_pointer_cast<Or>(subformulas[idx])) {
        updateVector[idx] = applyBooleanOperation(idx, subformulas, updateVector,
                                                  [&](const auto &left, const auto &right) { return left or right; });
    }

    // Xor
    else if (std::dynamic_pointer_cast<Xor>(subformulas[idx])) {
        updateVector[idx] = applyBooleanOperation(idx, subformulas, updateVector,
                                                  [&](const auto &left, const auto &right) { return left xor right; });
    }

    // Implication
    else if (std::dynamic_pointer_cast<Implication>(subformulas[idx])) {
        updateVector[idx] = applyBooleanOperation(
            idx, subformulas, updateVector, [&](const auto &left, const auto &right) { return not left or right; });
    }

    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(subformulas[idx])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), globally->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        updateVector[idx] = updateVector[indexLeft] and oldVector[idx];
    }
}

std::map<std::string, std::vector<Verdict>> DynamicProgrammingLTL::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    if (!pastEvaluation)
        result[formulas.at(0).getId()] = {Verdict{checkTraceDynamicallyFuture(trace)}};
    else
        result[formulas.at(0).getId()] = {Verdict{checkTraceDynamicallyPast(trace)}};
    return result;
}

std::vector<bool> DynamicProgrammingLTL::initializePre(Event &event,
                                                       const std::vector<std::shared_ptr<Expression>> &subformulas) {
    std::vector<bool> pre;
    event_map_t eventMap = event.getEventMap();
    pre.resize(subformulas.size());
    // Fill pre backwards
    for (size_t idx = subformulas.size() - 1; idx < subformulas.size(); --idx) {
        initialize(idx, pre, eventMap, subformulas);
        // Previously
        if (std::dynamic_pointer_cast<Previously>(subformulas[idx])) {
            pre[idx] = false;
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            pre[idx] = pre[indexLeft];
        }
    }

    return pre;
}

std::vector<bool> DynamicProgrammingLTL::updateNowPast(Event &event,
                                                       const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                       const std::vector<bool> &pre) {
    std::vector<bool> now;
    now.resize(pre.size());
    event_map_t eventMap = event.getEventMap();

    // idx >= 0 && idx < subformulas.size() to avoid underflow
    for (size_t idx = subformulas.size() - 1; idx < subformulas.size(); --idx) {
        update(idx, now, pre, eventMap, subformulas);
        // Previously
        if (auto previously = std::dynamic_pointer_cast<Previously>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), previously->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[idx] = pre[indexLeft];
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[idx] = now[indexLeft] or pre[idx];
        }
    }
    return now;
}

bool DynamicProgrammingLTL::checkTraceDynamicallyPast(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        return false;
    }
    std::vector<std::shared_ptr<Expression>> subformulae = Utils::subformulasBreadthFirstOrder(formulas.at(0));
    std::vector<bool> pre = initializePre(trace.getTraceRef()[0], subformulae);
    for (size_t i = 1; i < trace.getTrace().size(); ++i) {
        pre = updateNowPast(trace.getTraceRef()[i], subformulae, pre);
        // Only for safety properties!
        if (!pre[0]) {
            // std::cout << "Formula violated in event " << i - 1 << std::endl;
            return false;
        }
    }
    return pre[0];
}

std::vector<bool> DynamicProgrammingLTL::initializeNext(Event &event,
                                                        const std::vector<std::shared_ptr<Expression>> &subformulas) {
    std::vector<bool> next;
    event_map_t eventMap = event.getEventMap();
    next.resize(subformulas.size());
    // Fill next backwards
    for (size_t idx = subformulas.size() - 1; idx < subformulas.size(); --idx) {
        initialize(idx, next, eventMap, subformulas);
        // Weak Next
        if (std::dynamic_pointer_cast<NextWeak>(subformulas[idx])) {
            next[idx] = true;
        }
        // Strong Next
        else if (std::dynamic_pointer_cast<NextStrong>(subformulas[idx])) {
            next[idx] = false;
        }
        // Future
        else if (std::dynamic_pointer_cast<Future>(subformulas[idx])) {
            auto future = std::dynamic_pointer_cast<Future>(subformulas[idx]);
            auto itr = std::find(subformulas.begin(), subformulas.end(), future->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            next[idx] = next[indexLeft];
        }

        // Until
        else if (auto until = std::dynamic_pointer_cast<Until>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), until->getRight());
            auto indexRight = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            next[idx] = next[indexRight];
        }
    }

    return next;
}

std::vector<bool> DynamicProgrammingLTL::updateNowFuture(Event &event,
                                                         const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                         const std::vector<bool> &next) {
    std::vector<bool> now;
    now.resize(next.size());
    event_map_t eventMap = event.getEventMap();

    // idx >= 0 && idx < subformulas.size() to avoid underflow
    for (size_t idx = subformulas.size() - 1; idx < subformulas.size(); --idx) {
        update(idx, now, next, eventMap, subformulas);
        // Next
        if (std::dynamic_pointer_cast<NextStrong>(subformulas[idx]) ||
            std::dynamic_pointer_cast<NextWeak>(subformulas[idx])) {
            auto unary = std::dynamic_pointer_cast<Unary>(subformulas[idx]);
            auto itr = std::find(subformulas.begin(), subformulas.end(), unary->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[idx] = next[indexLeft];
        }
        // Future
        else if (auto future = std::dynamic_pointer_cast<Future>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), future->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[idx] = now[indexLeft] or next[idx];
        }

        // Until
        else if (auto until = std::dynamic_pointer_cast<Until>(subformulas[idx])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), until->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            itr = std::find(subformulas.begin(), subformulas.end(), until->getRight());
            auto indexRight = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[idx] = now[indexRight] or (now[indexLeft] and next[idx]);
        }
    }
    return now;
}

bool DynamicProgrammingLTL::checkTraceDynamicallyFuture(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        return false;
    }
    std::vector<std::shared_ptr<Expression>> subformulas = Utils::subformulasBreadthFirstOrder(formulas.at(0));
    std::vector<bool> next = initializeNext(trace.getTraceRef()[trace.getLength() - 1], subformulas);
    if (!next[0]) {
        return false;
    }
    for (size_t i = trace.getLength() - 2; i < trace.getLength() - 1; --i) {
        next = updateNowFuture(trace.getTraceRef()[i], subformulas, next);
        // Only for safety properties!
        if (!next[0]) {
            return false;
        }
    }
    return next[0];
}
std::string DynamicProgrammingLTL::to_string() const { return "TODO"; }

bool DynamicProgrammingLTL::isPastEvaluation() const { return pastEvaluation; }
