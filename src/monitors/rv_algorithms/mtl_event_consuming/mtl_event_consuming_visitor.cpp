//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "mtl_event_consuming_visitor.h"
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

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAtomic(Atomic &op, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPredicate(Predicate &op, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitConstant(Constant &op, Trace &trace, size_t index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAnd(And &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOr(Or &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitXor(Xor &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitImplication(Implication &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEquivalence(Equivalence &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNot(Not &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitFuture(Future &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitGlobally(Globally &op, Trace &trace, size_t index) {
    // visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitHistorically(Historically &op, Trace &trace, size_t index) {
    // visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextStrong(NextStrong &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitNextWeak(NextWeak &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitOnce(Once &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPreviously(Previously &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitRelease(Release &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitUntil(Until &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Until>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitSince(Since &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                            size_t index) {
    return std::make_shared<DerivingOperator>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                             size_t index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitEventOperator(EventOperator &op, Trace &trace,
                                                                         size_t index) {

    std::shared_ptr<Expression> ret;
    // Apply eventMap to Atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
    }
    // Constant
    else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
    // Predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*predicate));
    }
    // Deriving
    else if (auto deriving = std::dynamic_pointer_cast<DerivingOperator>(op.getLeft())) {
        ret = std::make_shared<EventOperator>(deriving->getLeft(), trace, index);
    }
    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);

    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            auto nextWeakExpr = std::make_shared<EventOperator>(nextWeak->getLeft(), trace, index);
            ret = nextWeak->accept(*this, trace, index + 1);
        } else {
            ret = std::make_shared<Constant>(true);
        }
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            auto nextStrongExpr = std::make_shared<EventOperator>(nextStrong->getLeft(), trace, index);
            ret = nextStrong->accept(*this, trace, index + 1);
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        size_t trace_length = trace.getLength();
        auto interval_start = static_cast<size_t>(until->getInterval()->getStart());
        auto interval_end = static_cast<size_t>(until->getInterval()->getEnd());

        if (index + interval_start >= trace_length) { // we can't have a look at all at the trace
            ret = std::make_shared<Constant>(false);
        } else { // we can have a look at the trace
            auto interval_length = (interval_end - interval_start) + 1;
            auto remaining_trace_length = (trace_length - (index + interval_start));
            auto min = (remaining_trace_length < interval_length) ? remaining_trace_length : interval_length;

            And::SetType_T andSet{};
            /** Check if the left expr holds from current time step to when the interval starts **/
            for (size_t k = 0; k < interval_start; ++k) {
                auto evalExprK = std::make_shared<EventOperator>(until->getLeft(), trace, index);
                auto evalExprKSol = evalExprK->accept(*this, trace, index + k);
                andSet.emplace(evalExprKSol);
            }
            /** Check if the right expr holds at time steps t + i, ensuring the left expr holds for the earlier time
             * steps **/
            And::SetType_T andSet_i{};
            Or::SetType_T orSet_i{};
            auto evalExprLeftInitial = std::make_shared<EventOperator>(until->getRight(), trace, index);
            auto evalExprLeftInitialSol = evalExprLeftInitial->accept(*this, trace, index + interval_start + min - 1);
            orSet_i.emplace(evalExprLeftInitialSol);
            for (size_t i{min - 1}; i >= 0; --i) {
                andSet_i.clear();
                auto evalExprLeft = std::make_shared<EventOperator>(until->getLeft(), trace, index);
                auto evalExprLeftSol = evalExprLeft->accept(*this, trace, index + interval_start + i);
                andSet_i.emplace(evalExprLeftSol);
                andSet_i.emplace(std::make_shared<Or>(orSet_i));

                orSet_i.clear();
                auto evalExprRight = std::make_shared<EventOperator>(until->getRight(), trace, index);
                auto evalExprRightSol = evalExprRight->accept(*this, trace, index + interval_start + i);
                orSet_i.emplace(evalExprRightSol);
                orSet_i.emplace(std::make_shared<And>(andSet_i));
            }
            auto orExprAsIntervalWithZero = std::make_shared<Or>(orSet_i);
            andSet.emplace(orExprAsIntervalWithZero);
            ret = std::make_shared<And>(andSet);
        }
    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        size_t trace_length = trace.getLength();
        auto interval_start = since->getInterval()->getStart();
        auto interval_end = since->getInterval()->getEnd();
        long difference = static_cast<long>(index) - interval_start;

        if (difference < 0) { // we can't have a look at all at the trace
            ret = std::make_shared<Constant>(false);
        } else { // we can have a look at the trace
            auto interval_length = (interval_end - interval_start) + 1;
            long remaining_trace_length = static_cast<long>(index) + 1 - interval_start;
            auto min = (remaining_trace_length < interval_length) ? remaining_trace_length : interval_length;
            And::SetType_T andSet{};
            /** Check if the left expr holds from current time step to when the interval starts **/
            for (size_t k = 0; k < interval_start; ++k) {
                auto evalExprK = std::make_shared<EventOperator>(since->getLeft(), trace, index);
                auto evalExprKSol = evalExprK->accept(*this, trace, index - k);
                andSet.emplace(evalExprKSol);
            }
            /** Check if the right expr holds at time steps t - i, ensuring the left expr holds for the later time steps
             * **/
            And::SetType_T andSet_i{};
            Or::SetType_T orSet_i{};
            auto evalExprLeftInitial = std::make_shared<EventOperator>(since->getRight(), trace, index);
            auto evalExprLeftInitialSol = evalExprLeftInitial->accept(*this, trace, index - interval_start - min + 1);
            orSet_i.emplace(evalExprLeftInitialSol);
            for (int i = min - 1; i >= 0; --i) {
                andSet_i.clear();
                auto evalExprLeft = std::make_shared<EventOperator>(since->getLeft(), trace, index);
                auto evalExprLeftSol = evalExprLeft->accept(*this, trace, index - interval_start - i);
                andSet_i.emplace(evalExprLeftSol);
                andSet_i.emplace(std::make_shared<Or>(orSet_i));

                orSet_i.clear();
                auto evalExprRight = std::make_shared<EventOperator>(since->getRight(), trace, index);
                auto evalExprRightSol = evalExprRight->accept(*this, trace, index - interval_start - i);
                orSet_i.emplace(evalExprRightSol);
                orSet_i.emplace(std::make_shared<And>(andSet_i));
            }
            auto orExprAsIntervalWithZero = std::make_shared<Or>(orSet_i);
            andSet.emplace(orExprAsIntervalWithZero);
            ret = std::make_shared<And>(andSet);
        }
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            if (globally->getInterval()->containsZero()) {
                And::SetType_T newSet;
                auto eventOperatorExpr = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
                newSet.emplace(eventOperatorExpr);
                globally->reduceInterval();
                newSet.emplace(globally);
                ret = std::make_shared<And>(newSet);
            } else {
                globally->reduceInterval();
                ret = globally;
            }
        } else {
            ret = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
        }
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        if (index > 0) {
            auto prevExprOpr = std::make_shared<EventOperator>(previously->getLeft(), trace, index /* - 1 */);
            ret = prevExprOpr->accept(*this, trace, index - 1);
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
    }
    // Historically
    else if (auto historically = std::dynamic_pointer_cast<Historically>(op.getLeft())) {
    }
    return ret->accept(*this, trace, index);
}