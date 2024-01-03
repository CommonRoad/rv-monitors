#pragma once

#include "btt_fsm.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/mt_fsm/multi_transition.h"
#include <algorithm>

/**
 * Encapsulates all functions needed to create a BTT-FSM from a formula.
 */
namespace BTT_FSMUtilsCreation {

/**
 * Replaces an atomic with a constant in a given formula. This is used to determine the next node in the BTT.
 * @param formula Formula.
 * @param atomic Atomic to be replaced.
 * @param constant Constant atomic is to be replaced with.
 * @return Formula with replaced atomic.
 */
std::shared_ptr<Expression> replaceAtomicByConstant(const Formula &formula, const std::shared_ptr<Atomic> &atomic,
                                                    bool constant);

MultiTransition replaceAtomicInMultiTransition(const MultiTransition &multiTransition,
                                               const std::shared_ptr<Atomic> &atomic, bool constant);
/**
 * Erases a given atomic from all formulas in a MultiTransition.
 * @param multiTransition MultiTransition.
 * @param atomic Atomic to be erased.
 * @return MultiTransition where formulas do not contain atomic anymore.
 */
MultiTransition eraseAtomicFromFormula(const MultiTransition &multiTransition, const std::shared_ptr<Atomic> &atomic);

/**
 * Used for building the BTT in each state. Checks which atomic (Node) should be added next by replacing each
 * atomic with true/false in each formula of multiTransition. Only one atomic is replaced at a time.
 * If one of the formulas evaluates to true after this the replaced atomic as a Node is the next state.
 * @param multiTransition MultiTransition.
 * @param weights Weights of the atomics.
 * @return Node that should be added to the BTT next.
 */
std::shared_ptr<node> findNextNode(const MultiTransition &multiTransition,
                                   std::multimap<double, std::shared_ptr<Atomic>> &weights);

} // namespace BTT_FSMUtilsCreation
