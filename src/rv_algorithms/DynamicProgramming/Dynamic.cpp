#include "Dynamic.h"
#include "DynamicFuture.h"
#include "DynamicPast.h"
#include "expression_tree/atomic/atomic.h"
#include "expression_tree/atomic/constant.h"
#include "expression_tree/binary/bool/implication.h"
#include "expression_tree/nary/bool/and.h"
#include "expression_tree/nary/bool/or.h"
#include "expression_tree/nary/bool/xor.h"
#include "expression_tree/unary/bool/not.h"
#include "expression_tree/unary/temporal/ltlf/globally.h"
#include "expression_tree/unary/temporal/ptlf/once.h"
#include "expression_tree/unary/temporal/ptlf/previously.h"
#include <utils/lazy_bool.h>

void Dynamic::initialize(size_t j, std::vector<bool> &initVector, std::map<std::string, lazy_bool> &eventMap,
                         const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // TODO evaluate predicates
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[j])) {
        initVector[j] = eventMap.at(atomic->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[j])) {
        initVector[j] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = std::distance(subformulas.begin(), itr);
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
        auto indexLeft = std::distance(subformulas.begin(), itr);
        initVector[j] = initVector[indexLeft];
    }
}

void Dynamic::update(size_t j, std::vector<bool> &updateVector, std::vector<bool> oldVector,
                     std::map<std::string, lazy_bool> &eventMap,
                     const std::vector<std::shared_ptr<Expression>> &subformulas) {
    // Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(subformulas[j])) {
        updateVector[j] = eventMap.at(atomic->to_string());
    }
    // Constant
    if (auto constant = std::dynamic_pointer_cast<Constant>(subformulas[j])) {
        updateVector[j] = constant->getValue();
    }
    // Not
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(subformulas[j])) {
        auto itr = std::find(subformulas.begin(), subformulas.end(), notExpr->getLeft());
        auto indexLeft = std::distance(subformulas.begin(), itr);
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
        auto indexLeft = std::distance(subformulas.begin(), itr);
        updateVector[j] = updateVector[indexLeft] and oldVector[j];
    }
}

bool Dynamic::dynamic(const Formula &formula, Trace &trace, bool past) {
    if (!past) {
        return DynamicFuture::checkTraceDynamically(formula, trace);
    }
    return DynamicPast::checkTraceDynamically(formula, trace);
}
