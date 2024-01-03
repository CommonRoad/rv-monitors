//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "monitors/rv_algorithms/mtl_event_consuming//mtl_event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/release.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/deriving_operator.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/first_order/counting_quantifier.h"
#include "monitors/expression_tree/unary/first_order/existential_quantifier.h"
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
#include <iostream>

void MTLEventConsumingVisitor::consumeEvent(Formula &formula, Trace &trace, size_t index) {
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAtomic(Atomic &opr, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPredicate(Predicate &opr, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitConstant(Constant &opr, Trace &trace, size_t index) {
    return std::make_shared<Constant>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAnd(And &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<And>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOr(Or &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Or>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitXor(Xor &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Xor>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitImplication(Implication &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Implication>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEquivalence(Equivalence &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Equivalence>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNot(Not &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Not>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitFuture(Future &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Future>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitGlobally(Globally &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Globally>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitHistorically(Historically &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Historically>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextStrong(NextStrong &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextStrong>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextWeak(NextWeak &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextWeak>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOnce(Once &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Once>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPreviously(Previously &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Previously>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitRelease(Release &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Release>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitUntil(Until &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Until>(opr.getLeft(), opr.getRight(), opr.getInterval());
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitSince(Since &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Since>(opr.getLeft(), opr.getRight(), opr.getInterval());
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitDerivingOperator(DerivingOperator &opr, Trace &trace,
                                                                            size_t index) {
    return std::make_shared<DerivingOperator>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitResolvingOperator(ResolvingOperator &opr, Trace &trace,
                                                                             size_t index) {
    return std::make_shared<ResolvingOperator>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEventOperator(EventOperator &opr, Trace &trace,
                                                                         size_t index) {
    std::shared_ptr<Expression> ret;
    // Apply eventMap to Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(opr.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
    }
    // Constant
    else if (auto constant = std::dynamic_pointer_cast<Constant>(opr.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
    // Predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(opr.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*predicate));
    }
    // Deriving
    else if (auto deriving = std::dynamic_pointer_cast<DerivingOperator>(opr.getLeft())) {
        ret = std::make_shared<EventOperator>(deriving->getLeft(), trace, index);
    }
    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(opr.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(opr.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(opr.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);

    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(opr.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            auto nextWeakExpr = std::make_shared<EventOperator>(nextWeak->getLeft(), trace, index);
            ret = nextWeakExpr->accept(*this, trace, index + 1);
        } else {
            ret = std::make_shared<Constant>(true);
        }
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(opr.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            auto nextStrongExpr = std::make_shared<EventOperator>(nextStrong->getLeft(), trace, index);
            ret = nextStrongExpr->accept(*this, trace, index + 1);
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(opr.getLeft())) {
        if ((!until->getInterval()->isFinite() or until->getInterval()->getEnd() != 0) and
            !trace.getEvent(index).isTerminal()) {
            if (until->getInterval()->getStart() > 0) {
                And::SetType_T andSet;
                auto exprLeft = std::make_shared<EventOperator>(until->getLeft(), trace, index);
                andSet.emplace(exprLeft);
                std::shared_ptr<Until> newUntil;
                if (until->getInterval()->isFinite())
                    newUntil = std::make_shared<Until>(until->getLeft(), until->getRight(),
                                                       std::make_shared<Interval>(until->getInterval()->getStart() - 1,
                                                                                  until->getInterval()->getEnd() - 1));
                else
                    newUntil =
                        std::make_shared<Until>(until->getLeft(), until->getRight(),
                                                std::make_shared<Interval>(until->getInterval()->getStart() - 1));
                auto newUntilExpr = std::make_shared<EventOperator>(newUntil, trace, index + 1);
                auto newUntilExpr_resolved = newUntilExpr->accept(*this, trace, index + 1);
                andSet.emplace(newUntilExpr_resolved);
                ret = std::make_shared<And>(andSet);
            } else {
                Or::SetType_T orSet;
                auto exprRight = std::make_shared<EventOperator>(until->getRight(), trace, index);
                orSet.emplace(exprRight);

                And::SetType_T andSet;
                auto exprLeft = std::make_shared<EventOperator>(until->getLeft(), trace, index);
                andSet.emplace(exprLeft);
                std::shared_ptr<Until> newUntil;
                if (until->getInterval()->isFinite())
                    newUntil = std::make_shared<Until>(until->getLeft(), until->getRight(),
                                                       std::make_shared<Interval>(until->getInterval()->getStart() - 1,
                                                                                  until->getInterval()->getEnd() - 1));
                else
                    newUntil =
                        std::make_shared<Until>(until->getLeft(), until->getRight(),
                                                std::make_shared<Interval>(until->getInterval()->getStart() - 1));
                auto newUntilExpr = std::make_shared<EventOperator>(newUntil, trace, index + 1);
                auto newUntilExpr_resolved = newUntilExpr->accept(*this, trace, index + 1);
                andSet.emplace(newUntilExpr_resolved);
                orSet.emplace(std::make_shared<And>(andSet));
                ret = std::make_shared<Or>(orSet);
            }
        } else {
            if ((until->getInterval()->isFinite() and until->getInterval()->getEnd() < 0) or
                until->getInterval()->getStart() > 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(until->getRight(), trace, index);
        }
    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(opr.getLeft())) {
        if ((!since->getInterval()->isFinite() or since->getInterval()->getEnd() != 0) and index > 0) {
            if (since->getInterval()->getStart() > 0) {
                And::SetType_T andSet;
                auto exprLeft = std::make_shared<EventOperator>(since->getLeft(), trace, index);
                andSet.emplace(exprLeft);
                std::shared_ptr<Since> newSince;
                if (since->getInterval()->isFinite())
                    newSince = std::make_shared<Since>(since->getLeft(), since->getRight(),
                                                       std::make_shared<Interval>(since->getInterval()->getStart() - 1,
                                                                                  since->getInterval()->getEnd() - 1));
                else
                    newSince =
                        std::make_shared<Since>(since->getLeft(), since->getRight(),
                                                std::make_shared<Interval>(since->getInterval()->getStart() - 1));
                auto newSinceExpr = std::make_shared<EventOperator>(newSince, trace, index - 1);
                auto newSinceExpr_resolved = newSinceExpr->accept(*this, trace, index - 1);
                andSet.emplace(newSinceExpr_resolved);
                ret = std::make_shared<And>(andSet);
            } else {
                Or::SetType_T orSet;
                auto exprRight = std::make_shared<EventOperator>(since->getRight(), trace, index);
                orSet.emplace(exprRight);

                And::SetType_T andSet;
                auto exprLeft = std::make_shared<EventOperator>(since->getLeft(), trace, index);
                andSet.emplace(exprLeft);
                std::shared_ptr<Since> newSince;
                if (since->getInterval()->isFinite())
                    newSince = std::make_shared<Since>(since->getLeft(), since->getRight(),
                                                       std::make_shared<Interval>(since->getInterval()->getStart() - 1,
                                                                                  since->getInterval()->getEnd() - 1));
                else
                    newSince =
                        std::make_shared<Since>(since->getLeft(), since->getRight(),
                                                std::make_shared<Interval>(since->getInterval()->getStart() - 1));
                auto newSinceExpr = std::make_shared<EventOperator>(newSince, trace, index - 1);
                auto newSinceExpr_resolved = newSinceExpr->accept(*this, trace, index - 1);
                andSet.emplace(newSinceExpr_resolved);
                orSet.emplace(std::make_shared<And>(andSet));
                ret = std::make_shared<Or>(orSet);
            }
        } else {
            if (since->getInterval()->isFinite() and since->getInterval()->getEnd() < 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(since->getRight(), trace, index);
        }
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(opr.getLeft())) {
        if (index > 0) {
            auto prevExprOpr = std::make_shared<EventOperator>(previously->getLeft(), trace, index);
            ret = prevExprOpr->accept(*this, trace, index - 1);
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft())) {
        // rewritten in mtl_rewrite_visitor
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
        // rewritten in mtl_rewrite_visitor
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(opr.getLeft())) {
        // rewritten in mtl_rewrite_visitor
    }
    // Historically
    else if (auto historically = std::dynamic_pointer_cast<Historically>(opr.getLeft())) {
        // rewritten in mtl_rewrite_visitor
    }
    return ret->accept(*this, trace, index);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitUniversalQuantifier(UniversalQuantifier &opr, Trace &trace,
                                                                               size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<UniversalQuantifier>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitExistentialQuantifier(ExistentialQuantifier &opr,
                                                                                 Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<ExistentialQuantifier>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitQuoteQuantifier(QuoteQuantifier &opr, Trace &trace,
                                                                           size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<QuoteQuantifier>(opr);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitCountingQuantifier(CountingQuantifier &opr, Trace &trace,
                                                                              size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<CountingQuantifier>(opr);
}
