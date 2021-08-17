#include "dynamic.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/ltlf/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include <monitors/utils/lazy_bool.h>

DynamicProgrammingLTL::DynamicProgrammingLTL(const Formula &formula, bool pastEvaluation)
    : Monitor((formula)), pastEvaluation(pastEvaluation) {}

void DynamicProgrammingLTL::initialize(size_t j, std::vector<bool> &initVector,
                                       std::map<std::string, lazy_bool> &eventMap,
                                       const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[j])) {
        initVector[j] = eventMap.at(atomic->to_string());
    }
    // Predicate
    if (auto predicate = std::dynamic_pointer_cast<Predicate>(subformulas[j])) {
        initVector[j] = eventMap.at(predicate->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[j])) {
        initVector[j] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        initVector[j] = not initVector[indexLeft];
    }

    // And
    else if (std::dynamic_pointer_cast<And>(subformulas[j])) {
        initVector[j] = applyBooleanOperation(j, subformulas, initVector,
                                              [&](const auto &left, const auto &right) { return left and right; });
    }
    // Or
    else if (std::dynamic_pointer_cast<Or>(subformulas[j])) {
        initVector[j] = applyBooleanOperation(j, subformulas, initVector,
                                              [&](const auto &left, const auto &right) { return left or right; });
    }

    // Xor
    else if (std::dynamic_pointer_cast<Xor>(subformulas[j])) {
        initVector[j] = applyBooleanOperation(j, subformulas, initVector,
                                              [&](const auto &left, const auto &right) { return left xor right; });
    }

    // Implication
    else if (std::dynamic_pointer_cast<Implication>(subformulas[j])) {
        initVector[j] = applyBooleanOperation(j, subformulas, initVector,
                                              [&](const auto &left, const auto &right) { return not left or right; });
    }

    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), globally->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        initVector[j] = initVector[indexLeft];
    }
}

void DynamicProgrammingLTL::update(size_t j, std::vector<bool> &updateVector, std::vector<bool> oldVector,
                                   std::map<std::string, lazy_bool> &eventMap,
                                   const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[j])) {
        updateVector[j] = eventMap.at(atomic->to_string());
    }
    // Predicate
    if (auto predicate = std::dynamic_pointer_cast<Predicate>(subformulas[j])) {
        updateVector[j] = eventMap.at(predicate->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[j])) {
        updateVector[j] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        updateVector[j] = not updateVector[indexLeft];
    }

    // And
    else if (std::dynamic_pointer_cast<And>(subformulas[j])) {
        updateVector[j] = applyBooleanOperation(j, subformulas, updateVector,
                                                [&](const auto &left, const auto &right) { return left and right; });
    }
    // Or
    else if (std::dynamic_pointer_cast<Or>(subformulas[j])) {
        updateVector[j] = applyBooleanOperation(j, subformulas, updateVector,
                                                [&](const auto &left, const auto &right) { return left or right; });
    }

    // Xor
    else if (std::dynamic_pointer_cast<Xor>(subformulas[j])) {
        updateVector[j] = applyBooleanOperation(j, subformulas, updateVector,
                                                [&](const auto &left, const auto &right) { return left xor right; });
    }

    // Implication
    else if (std::dynamic_pointer_cast<Implication>(subformulas[j])) {
        updateVector[j] = applyBooleanOperation(j, subformulas, updateVector,
                                                [&](const auto &left, const auto &right) { return not left or right; });
    }

    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), globally->getLeft());
        auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
        updateVector[j] = updateVector[indexLeft] and oldVector[j];
    }
}

std::vector<Verdict> DynamicProgrammingLTL::evaluate(Trace &trace) {
    if (!pastEvaluation) {
        return {Verdict{0, checkTraceDynamicallyFuture(trace)}};
    }
    return {Verdict{0, checkTraceDynamicallyPast(trace)}};
}

std::vector<bool> DynamicProgrammingLTL::initializePre(Event &event,
                                                       const std::vector<std::shared_ptr<Expression>> &subformulas) {
    std::vector<bool> pre;
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();
    pre.resize(subformulas.size());
    // Fill pre backwards
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        initialize(j, pre, eventMap, subformulas);
        // Previously
        if (std::dynamic_pointer_cast<Previously>(subformulas[j])) {
            pre[j] = true;
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            pre[j] = pre[indexLeft];
        }
    }

    return pre;
}

std::vector<bool> DynamicProgrammingLTL::updateNowPast(Event &event,
                                                       const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                       const std::vector<bool> &pre) {
    std::vector<bool> now;
    now.resize(pre.size());
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();

    // j >= 0 && j < subformulas.size() to avoid underflow
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        update(j, now, pre, eventMap, subformulas);
        // Previously
        if (auto previously = std::dynamic_pointer_cast<Previously>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), previously->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[j] = pre[indexLeft];
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[j] = now[indexLeft] or pre[j];
        }
    }
    return now;
}

bool DynamicProgrammingLTL::checkTraceDynamicallyPast(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        return false;
    }
    std::vector<std::shared_ptr<Expression>> subformulae = Utils::subformulasBreadthFirstOrder(formula);
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
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();
    next.resize(subformulas.size());
    // Fill next backwards
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        initialize(j, next, eventMap, subformulas);
        // Weak Next
        if (std::dynamic_pointer_cast<NextWeak>(subformulas[j])) {
            next[j] = true;
        }
        // Strong Next
        else if (std::dynamic_pointer_cast<NextStrong>(subformulas[j])) {
            next[j] = false;
        }
        // Future
        else if (std::dynamic_pointer_cast<Future>(subformulas[j])) {
            auto future = std::dynamic_pointer_cast<Future>(subformulas[j]);
            auto itr = std::find(subformulas.begin(), subformulas.end(), future->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            next[j] = next[indexLeft];
        }

        // Until
        else if (auto until = std::dynamic_pointer_cast<Until>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), until->getRight());
            auto indexRight = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            next[j] = next[indexRight];
        }
    }

    return next;
}

std::vector<bool> DynamicProgrammingLTL::updateNowFuture(Event &event,
                                                         const std::vector<std::shared_ptr<Expression>> &subformulas,
                                                         const std::vector<bool> &next) {
    std::vector<bool> now;
    now.resize(next.size());
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();

    // j >= 0 && j < subformulas.size() to avoid underflow
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        update(j, now, next, eventMap, subformulas);
        // Next
        if (std::dynamic_pointer_cast<NextStrong>(subformulas[j]) ||
            std::dynamic_pointer_cast<NextWeak>(subformulas[j])) {
            auto unary = std::dynamic_pointer_cast<Unary>(subformulas[j]);
            auto itr = std::find(subformulas.begin(), subformulas.end(), unary->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[j] = next[indexLeft];
        }
        // Future
        else if (auto future = std::dynamic_pointer_cast<Future>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), future->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[j] = now[indexLeft] or next[j];
        }

        // Until
        else if (auto until = std::dynamic_pointer_cast<Until>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), until->getLeft());
            auto indexLeft = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            itr = std::find(subformulas.begin(), subformulas.end(), until->getRight());
            auto indexRight = static_cast<size_t>(std::distance(subformulas.begin(), itr));
            now[j] = now[indexRight] or (now[indexLeft] and next[j]);
        }
    }
    return now;
}

bool DynamicProgrammingLTL::checkTraceDynamicallyFuture(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        return false;
    }
    std::vector<std::shared_ptr<Expression>> subformulas = Utils::subformulasBreadthFirstOrder(formula);
    std::vector<bool> next = initializeNext(trace.getTraceRef()[trace.getLength() - 1], subformulas);
    if (!next[0]) {
        return false;
    }
    for (size_t i = trace.getLength() - 2; i >= 0 && i < trace.getLength() - 1; --i) {
        next = updateNowFuture(trace.getTraceRef()[i], subformulas, next);
        // Only for safety properties!
        if (!next[0]) {
            return false;
        }
    }
    return next[0];
}
std::string DynamicProgrammingLTL::to_string() const { return "TODO"; }
