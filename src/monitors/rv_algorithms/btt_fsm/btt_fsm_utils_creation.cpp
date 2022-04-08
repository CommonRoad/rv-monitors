#include "btt_fsm_utils_creation.h"
#include "decision.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/nary/nary_interface.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/rewriting/basic_rewrite_visitor.h"
#include "monitors/utils/utils.h"
#include <iostream>

std::shared_ptr<Expression> BTT_FSMUtilsCreation::replaceAtomicByConstant(const Formula &formula,
                                                                          const std::shared_ptr<Atomic> &atomic,
                                                                          bool constant) {
    auto expression = formula.getExpr();

    // Single atomic
    if (auto atomicExpr = std::dynamic_pointer_cast<Atomic>(expression)) {
        if (*atomicExpr == *atomic) {
            return std::make_shared<Constant>(constant);
        } else {
            throw std::runtime_error(
                "BTT_FSMCreation::replaceAtomicByConstantTrue Single atomic that is not the searched one.");
        }
    }

    // Not atomic
    else if (auto notExpr = std::dynamic_pointer_cast<Not>(expression)) {
        if (*notExpr->getLeft() == *atomic) {
            return std::make_shared<Constant>(!constant);
        }
    }
    // We have an n-ary expression
    auto nary = std::dynamic_pointer_cast<NaryInterface>(expression);

    // If we now don't have an n-ary expression something is really wrong
    if (not nary) {
        throw std::runtime_error("BTT_FSMCreation::replaceAtomicByConstantTrue Expression is not n-ary but should be.");
    }

    // Find atomic and replace it, we definitely don't have fewer than two elements in nary
    auto itr = nary->findExpression(atomic);
    if (itr != nary->end()) {
        nary->eraseExpression(itr);
        nary->addExpression(std::make_shared<Constant>(constant));
        // std::cout << "Found " << atomic->to_string() << std::endl;
    }
    // Atomic could be negated
    else if (auto itrNot = nary->findExpression(std::make_shared<Not>(atomic)); itrNot != nary->end()) {
        nary->eraseExpression(itrNot);
        nary->addExpression(std::make_shared<Constant>(!constant));
        // std::cout << "Found !" << atomic->to_string() << std::endl;
    }

    // Rewrite to reduce if possible
    Formula newFormula(nary);
    BasicRewriteVisitor br;
    br.rewrite(newFormula);
    //    std::cout << "Result of " << atomic->to_string() << " replaced with " << constant << " in " <<
    //    formula.to_string()
    //              << ": " << newFormula.to_string() << std::endl;
    return newFormula.getExpr();
}

MultiTransition BTT_FSMUtilsCreation::eraseAtomicFromFormula(const MultiTransition &multiTransition,
                                                             const std::shared_ptr<Atomic> &atomic) {

    std::map<Formula, size_t> newTransitions;
    // To search for negated atomic
    auto notAtomic = std::make_shared<Not>(atomic);
    BasicRewriteVisitor br;
    for (const auto &[formula, state] : multiTransition.getTransitions()) {
        auto formulaCopy = formula.deepCopy();
        // Erase atomic from formulas
        // Check if only atomic
        if (auto atomicExpr = std::dynamic_pointer_cast<Atomic>(formulaCopy.getExpr())) {
            // Only searched atomic is left -> nothing to do
            if (not(*atomic == *atomicExpr)) {
                Formula newFormula(atomicExpr);
                newTransitions.emplace(newFormula, state);
            }
        }
        // Atomic could be negated
        else if (auto notExpr = std::dynamic_pointer_cast<Not>(formulaCopy.getExpr())) {
            if (not(*notAtomic == *notExpr)) {
                Formula newFormula(notExpr);
                newTransitions.emplace(newFormula, state);
            }
        }
        // Otherwise we have an n-ary
        else if (auto nary = std::dynamic_pointer_cast<NaryInterface>(formulaCopy.getExpr())) {
            nary->eraseExpression(atomic);
            nary->eraseExpression(notAtomic);
            Formula newFormula(nary);
            br.rewrite(newFormula);
            newTransitions.emplace(newFormula, state);
        } else {
            throw std::runtime_error(
                "BTT_FSMCreation::eraseAtomicFromFormula() Formula is neither Atomic, Not, or Nary.");
        }
    }
    return MultiTransition(newTransitions);
}

MultiTransition BTT_FSMUtilsCreation::replaceAtomicInMultiTransition(const MultiTransition &multiTransition,
                                                                     const std::shared_ptr<Atomic> &atomic,
                                                                     bool constant) {

    std::map<Formula, size_t> newTransitions;
    for (const auto &[formula, state] : multiTransition.getTransitions()) {
        auto newFormula = formula.deepCopy();
        replaceAtomicByConstant(newFormula, atomic, constant);
        newTransitions.emplace(newFormula, state);
    }
    return MultiTransition(newTransitions);
}

std::shared_ptr<node> BTT_FSMUtilsCreation::findNextNode(const MultiTransition &multiTransition,
                                                         std::multimap<double, std::shared_ptr<Atomic>> &weights) {
    // Constant
    if (multiTransition.getTransitions().size() == 1) {
        if (auto constantExpr =
                std::dynamic_pointer_cast<Constant>((multiTransition.getTransitions().begin()->first.getExpr()))) {
            size_t id = multiTransition.getTransitions().at(multiTransition.getTransitions().begin()->first);
            auto state = std::make_shared<StateBttFsm>(id);
            return std::make_shared<Decision>(state, state, constantExpr);
        }
        throw std::runtime_error("BTT_FSMCreation::findNextNode() Multitransition has size 1 and it's not a Constant.");
    }

    // Check all atomics
    for (auto itrPrecedences = weights.begin(); itrPrecedences != weights.end(); ++itrPrecedences) {
        const auto &[precedence, atomic] = *itrPrecedences;
        // In all formulas in multiTransition
        for (const auto &[formula, nextState] : multiTransition.getTransitions()) {
            if (formula.containsAtomic(atomic)) {
                for (bool constant : {true, false}) {
                    // std::cout << "Current atomic: " << atomic->to_string() << " evaluated as " << constant << " on "
                    // << formula.to_string() << std::endl;
                    auto formulaCopy = formula.deepCopy();
                    auto resExpr = replaceAtomicByConstant(formulaCopy, atomic, constant);
                    //                    std::cout << "Replacing " << atomic->to_string() << " with " << constant << "
                    //                    in " << formulaCopy.to_string() << " is "
                    //                              << resExpr->to_string() << std::endl;
                    // Check if replacing this atomic resolves the formula to true
                    if (resExpr->isConstTrue()) {
                        // Last atomic = base case
                        if (weights.size() == 1) {
                            auto left = std::make_shared<StateBttFsm>(nextState);

                            event_map_t eventMap;

                            eventMap.emplace(atomic->to_string(), lazy_bool(not constant));
                            auto right = std::make_shared<StateBttFsm>(multiTransition.evaluate(eventMap));
                            // Get left an right in order
                            if (constant) {
                                return std::make_shared<Decision>(left, right, atomic);
                            }
                            return std::make_shared<Decision>(right, left, atomic);
                        } else {
                            // Replace atomic with constant in all formulas by building new multiTransition
                            auto newMultitransition = eraseAtomicFromFormula(multiTransition, atomic);
                            // std::cout << "MT after removing " << atomic->to_string() << "\n" <<
                            // newMultitransition.to_string() << std::endl;
                            // Copy atomic since we erase it in the next step
                            auto atomicCopy = std::make_shared<Atomic>(atomic->to_string());
                            // Erase atomic from weights
                            weights.erase(itrPrecedences);
                            if (constant) {
                                return std::make_shared<Decision>(std::make_shared<StateBttFsm>(nextState),
                                                                  findNextNode(newMultitransition, weights),
                                                                  atomicCopy);
                            } else {
                                return std::make_shared<Decision>(findNextNode(newMultitransition, weights),
                                                                  std::make_shared<StateBttFsm>(nextState), atomicCopy);
                            }
                        }
                    }
                }
            }
        }
    }

    // No atomic was found that immediately solves the formula
    // Pick the one with the best precedence
    const auto &[_, atomic] = *weights.begin();

    auto newMultitransitionTrue = replaceAtomicInMultiTransition(multiTransition, atomic, true);
    auto newMultitransitionFalse = replaceAtomicInMultiTransition(multiTransition, atomic, false);

    auto atomicCopy = std::make_shared<Atomic>(atomic->to_string());
    weights.erase(weights.begin());

    return std::make_shared<Decision>(findNextNode(newMultitransitionTrue, weights),
                                      findNextNode(newMultitransitionFalse, weights), atomicCopy);
}
