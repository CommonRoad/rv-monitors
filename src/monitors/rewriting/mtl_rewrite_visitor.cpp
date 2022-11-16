//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "mtl_rewrite_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/binary.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/release.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/nary/nary_interface.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/deriving_operator.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/expression_tree/unary/first_order/existential_quantifier.h"
#include "monitors/expression_tree/unary/first_order/function/equality_operator.h"
#include "monitors/expression_tree/unary/first_order/quote_quantifier.h"
#include "monitors/expression_tree/unary/first_order/universal_quantifier.h"
#include "monitors/expression_tree/unary/resolving_operator.h"
#include "monitors/expression_tree/unary/temporal/future.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/expression_tree/unary/temporal/historically.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/next_weak.h"
#include "monitors/expression_tree/unary/temporal/once.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/rewriting/rewrite_visitor_utils.h"
#include "monitors/trace/event.h"
#include <iostream>

void MtlRewriteVisitor::rewrite(Formula &formula) {
    event_map_t map;
    // Dummy eventMap
    Event event(std::move(map), 0, false);
    auto trace = Trace(std::vector<Event>{event});

    auto before = Formula::deepCopy(formula.getExpr());
    formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    while (!Formula::sameTree(before, formula.getExpr())) {
        before = Formula::deepCopy(formula.getExpr());
        formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    }
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitAtomic(Atomic &opr, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitConstant(Constant &opr, Trace &trace, size_t index) {
    return std::make_shared<Constant>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitAnd(And &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);

    // I'm a copy!
    auto expressions = opr.getExpressions();

    // Check for false
    if (expressions.find(std::make_shared<Constant>(false)) != expressions.end()) {
        return std::make_shared<Constant>(false);
    }

    // Remove true
    auto ret = expressions.equal_range(std::make_shared<Constant>(true));
    expressions.erase(ret.first, ret.second);

    // If everything was removed there has only been a true -> return true
    if (expressions.empty()) {
        return std::make_shared<Constant>(true);
    }

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // Absorption a & (a | b) = a
    RewriteVisitorUtils::absorption<Or>(expressions);

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // !a & a = false
    RewriteVisitorUtils::reduceNegations<false>(expressions);

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // a & (b ^ c) = a & b ^ a & c
    for (auto itr = expressions.begin(); itr != expressions.end(); ++itr) {
        for (auto itrInner = expressions.begin(); itrInner != itr && itrInner != expressions.end(); ++itrInner) {
            // Found an Xor
            if (auto xorExpr = std::dynamic_pointer_cast<Xor>(*itrInner)) {
                Xor::SetType_T newXorExpr;
                for (const auto &exp : xorExpr->getExpressions()) {
                    // Create new Xor according to rule
                    newXorExpr.emplace(std::make_shared<And>(And::SetType_T({*itr, exp})));
                }
                // Add newXor to expressions
                expressions.emplace(std::make_shared<Xor>(newXorExpr));
                // Erase Xor and itr
                itrInner = expressions.erase(itrInner);
                itr = expressions.erase(itr);
                break;
            }
        }
    }

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    return std::make_shared<And>(expressions);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitOr(Or &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    auto expressions = opr.getExpressions();

    // Check for true
    if (expressions.find(std::make_shared<Constant>(true)) != expressions.end()) {
        return std::make_shared<Constant>(true);
    }
    // Remove false
    auto ret = expressions.equal_range(std::make_shared<Constant>(false));
    expressions.erase(ret.first, ret.second);

    // If everything was removed there has only been a false -> return false
    if (expressions.empty()) {
        return std::make_shared<Constant>(false);
    }

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // Absorption a | (a & b) = a
    RewriteVisitorUtils::absorption<And>(expressions);

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // !a | a = true
    RewriteVisitorUtils::reduceNegations<true>(expressions);

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    // a | b = a & b ^ a ^ b
    And::SetType_T andSet;
    Xor::SetType_T xorSet;

    auto iter = expressions.begin();
    andSet.emplace(*iter);
    xorSet.emplace(*iter++);
    andSet.emplace(*iter);
    xorSet.emplace(*iter);

    // Remove a and b
    expressions.erase(expressions.begin());
    expressions.erase(expressions.begin());

    auto andExpr = std::make_shared<And>(andSet);
    xorSet.emplace(andExpr);
    auto xorExpr = std::make_shared<Xor>(xorSet);

    // At least one element left
    if (!expressions.empty()) {
        expressions.emplace(xorExpr);
        return std::make_shared<Or>(expressions);
    }

    return xorExpr;

    // return std::make_shared<Or>(expressions);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitXor(Xor &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);

    auto expressions = opr.getExpressions();

    // !a ^ a = true
    RewriteVisitorUtils::reduceNegations<true>(expressions);

    Xor::SetType_T newXorSet;

    // Remove unnecessary duplicates
    for (auto itr = expressions.begin(); itr != expressions.end();) {
        // a ^ a = false
        if (expressions.count(*itr) % 2 == 0) {
            newXorSet.emplace(std::make_shared<Constant>(false));
        }
        // a ^ a ^ a = a
        else {
            newXorSet.emplace(*itr);
        }
        // Erase duplicates so we don't have to look at them again in the loop
        auto ret = expressions.equal_range(*itr);
        itr = expressions.erase(ret.first, ret.second);
    }

    // a ^ false = a -> remove false
    // Number of false does not matter
    auto ret = newXorSet.equal_range(std::make_shared<Constant>(false));
    newXorSet.erase(ret.first, ret.second);

    // Only false was left
    if (newXorSet.empty()) {
        return std::make_shared<Constant>(false);
    }
    // Only one element left
    if (newXorSet.size() == 1) {
        return *newXorSet.begin();
    }
    return std::make_shared<Xor>(newXorSet);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitImplication(Implication &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // a -> a = true
    if (*opr.getLeft() == *opr.getRight()) {
        return std::make_shared<Constant>(true);
    }
    // true -> false = false
    if (opr.getLeft()->isConstTrue() && opr.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // a -> true = true
    if (opr.getRight()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // false -> a = true
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }

    //    // a -> b = true ^ a ^ a & b
    //    And::SetType_T andSet{opr.getLeft(), opr.getRight()};
    //    auto andExpr = std::make_shared<And>(andSet);
    //    Xor::SetType_T xorSet{std::make_shared<Constant>(true), opr.getLeft(), andExpr};
    //    return std::make_shared<Xor>(xorSet);

    // a -> b = a v !b
    auto negExpr{std::make_shared<Not>(opr.getLeft())};
    Or::SetType_T orSet{opr.getRight(), negExpr};
    auto orExpr{std::make_shared<Or>(orSet)};
    return orExpr;
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitEquivalence(Equivalence &opr, Trace &trace, size_t index) {
    if (opr.getLeft() == opr.getRight()) {
        return std::make_shared<Constant>(true);
    }

    // a <-> b = true ^ a ^ b
    auto trueExpr = std::make_shared<Constant>(true);
    Xor::SetType_T xorSet{opr.getLeft(), opr.getRight(), trueExpr};
    return std::make_shared<Xor>(xorSet);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitNot(Not &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // !true = false
    if (opr.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(false);
    }
    // !false = true
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }

    // !a = true ^ a
    Xor::SetType_T xorSet{std::make_shared<Constant>(true), opr.getLeft()};
    return std::make_shared<Xor>(xorSet);
    //    return std::make_shared<Not>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitFuture(Future &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // F(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // Idempotence F(F(a)) = F(a)
    if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
        return future;
    }

    // Absorption F(G(F(a))) = G(F(a))
    if (auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft())) {
        if (std::dynamic_pointer_cast<Future>(globally->getLeft())) {
            return globally;
        }
    }

    //    // F(a) = !G(!a) <- might not be correct (not sure)
    //    auto negateFormula = std::make_shared<Not>(opr.getLeft());
    //    auto globalExpr = std::make_shared<Globally>(negateFormula, opr.getInterval());
    //    return std::make_shared<Not>(globalExpr);

    //     F(a) = true U a
    Until until{std::make_shared<Constant>(true), opr.getLeft(), opr.getInterval()};
    return std::make_shared<Until>(until);
    //       return std::make_shared<Future>(opr.getLeft(), opr.getInterval());
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitGlobally(Globally &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // G(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // G(true) = true
    if (opr.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    //        // Idempotence
    //        if ( auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft()) ) {
    //            return globally;
    //        }

    //    // Absorption G(F(G(a))) = F(G(a))
    //    if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
    //        if (std::dynamic_pointer_cast<Globally>(future->getLeft())) {
    //            return future;
    //        }
    //    }

    // G(a) = not F(not a) = true ^ F(true ^ a)
    //    Xor::SetType_T xorSetOne{std::make_shared<Constant>(true), opr.getLeft()};
    //    auto future {std::make_shared<Future>(std::make_shared<Xor>(xorSetOne), opr.getInterval())};
    //    Xor::SetType_T xorSetTwo{std::make_shared<Constant>(true), future};
    //    return std::make_shared<Xor>(xorSetTwo);

    //     G(a) = not F(not a) = ! (true U ! a)
    //        auto future{std::make_shared<Future>(std::make_shared<Not>(opr.getLeft()), opr.getInterval())};
    //        return std::make_shared<Not>(future);
    return std::make_shared<Not>(std::make_shared<Until>(std::make_shared<Constant>(true),
                                                         std::make_shared<Not>(opr.getLeft()), opr.getInterval()));

    //    return std::make_shared<Globally>(opr.getLeft(), opr.getInterval());
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitHistorically(Historically &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // H(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // H(true) = true
    if (opr.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // Idempotence
    if (auto historically = std::dynamic_pointer_cast<Historically>(opr.getLeft())) {
        return historically;
    }
    // Absorption H(O(H(a))) = O(H(a))
    if (auto once = std::dynamic_pointer_cast<Once>(opr.getLeft())) {
        if (std::dynamic_pointer_cast<Historically>(once->getLeft())) {
            return once;
        }
    }

    // H(a) = not O(not a) = true ^ O(true ^ a)
    //    Xor::SetType_T xorSetOne{std::make_shared<Constant>(true), opr.getLeft()};
    //    auto once {std::make_shared<Once>(std::make_shared<Xor>(xorSetOne), opr.getInterval())};
    //    Xor::SetType_T xorSetTwo{std::make_shared<Constant>(true), once};
    //
    //    return std::make_shared<Xor>(xorSetTwo);

    // H(a) = not O(not a)
    //    auto once{std::make_shared<Once>(std::make_shared<Not>(opr.getLeft()), opr.getInterval())};
    //    return std::make_shared<Not>(once);

    // H(a) = !a S a  // I dont think this rewriting is correct
    // H(a) = !(true S (!a))
    return std::make_shared<Not>(std::make_shared<Since>(std::make_shared<Constant>(true),
                                                         std::make_shared<Not>(opr.getLeft()), opr.getInterval()));

    return std::make_shared<Historically>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitNextStrong(NextStrong &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // X(false) = false
    auto false_constant = std::make_shared<Constant>(false);

    if (opr.getLeft()->isConstFalse()) {
        return false_constant;
    }

    //     X(a) = false U a
    // return std::make_shared<Until>(false_constant, opr.getLeft(), opr.getInterval());
    return std::make_shared<NextStrong>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitNextWeak(NextWeak &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextWeak>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitOnce(Once &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // O(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto once = std::dynamic_pointer_cast<Once>(opr.getLeft())) {
        return once;
    }

    // O(a) = true S a
    // Since since{std::make_shared<Constant>(true), opr.getLeft(), opr.getInterval()};
    return std::make_shared<Since>(std::make_shared<Constant>(true), opr.getLeft(), opr.getInterval());
    //     return std::make_shared<Once>(opr.getLeft(), opr.getInterval());
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitPreviously(Previously &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Previously>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitRelease(Release &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // a R true = true
    if (opr.getRight()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // a R false = false
    if (opr.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto release = std::dynamic_pointer_cast<Release>(opr.getLeft())) {
        if (release->getRight() == opr.getRight()) {
            return release;
        }
    }
    if (auto release = std::dynamic_pointer_cast<Release>(opr.getRight())) {
        if (release->getLeft() == opr.getLeft()) {
            return release;
        }
    }

    // f1 R f2 = !f1 U !f2
    return std::make_shared<Until>(std::make_shared<Not>(opr.getLeft()), std::make_shared<Not>(opr.getRight()),
                                   opr.getInterval());

    return std::make_shared<Release>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitUntil(Until &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // a U false = false
    if (opr.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto until = std::dynamic_pointer_cast<Until>(opr.getLeft())) {
        if (until->getRight() == opr.getRight()) {
            return until;
        }
    }
    if (auto until = std::dynamic_pointer_cast<Until>(opr.getRight())) {
        if (until->getLeft() == opr.getLeft()) {
            return until;
        }
    }
    return std::make_shared<Until>(opr.getLeft(), opr.getRight(), opr.getInterval());
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitSince(Since &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // a U false = false
    if (opr.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto since = std::dynamic_pointer_cast<Since>(opr.getLeft())) {
        if (since->getRight() == opr.getRight()) {
            return since;
        }
    }
    if (auto since = std::dynamic_pointer_cast<Since>(opr.getRight())) {
        if (since->getLeft() == opr.getLeft()) {
            return since;
        }
    }

    return std::make_shared<Since>(opr.getLeft(), opr.getRight(), opr.getInterval());
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitEventOperator(EventOperator &opr, Trace &trace, size_t index) {
    return std::make_shared<EventOperator>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitResolvingOperator(ResolvingOperator &opr, Trace &trace,
                                                                      size_t index) {
    return std::make_shared<ResolvingOperator>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitDerivingOperator(DerivingOperator &opr, Trace &trace,
                                                                     size_t index) {
    return std::make_shared<DerivingOperator>(opr);
}
std::shared_ptr<Expression> MtlRewriteVisitor::visitPredicate(Predicate &opr, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitUniversalQuantifier(UniversalQuantifier &opr, Trace &trace,
                                                                        size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<UniversalQuantifier>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitExistentialQuantifier(ExistentialQuantifier &opr, Trace &trace,
                                                                          size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<ExistentialQuantifier>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitQuoteQuantifier(QuoteQuantifier &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<QuoteQuantifier>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitCountingQuantifier(CountingQuantifier &opr, Trace &trace,
                                                                       size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<CountingQuantifier>(opr);
}

std::shared_ptr<Expression> MtlRewriteVisitor::visitEqualityOperator(EqualityOperator &opr, Trace &trace, size_t index) {
    return std::make_shared<EqualityOperator>(opr);
}
