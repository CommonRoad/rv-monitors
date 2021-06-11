#include "mtl_deriving_future_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/ltlf/release.h"
#include "monitors/expression_tree/binary/temporal/ltlf/until.h"
#include "monitors/expression_tree/binary/temporal/ptlf/since.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/deriving_operator.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/resolving_operator.h"
#include "monitors/expression_tree/unary/temporal/ltlf/future.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_strong.h"
#include "monitors/expression_tree/unary/temporal/ltlf/next_weak.h"
#include "monitors/expression_tree/unary/temporal/ptlf/historically.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include <iostream>

void MTLDerivingFutureVisitor::consumeEvent(Formula &formula, Trace &trace, int index) {
    std::shared_ptr<DerivingOperator> derivingOperator =
        std::make_shared<DerivingOperator>(formula.getExpr(), trace, index);
    formula.setExpr(derivingOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitAtomic(Atomic &op, Trace &trace, int index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitConstant(Constant &op, Trace &trace, int index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitAnd(And &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitOr(Or &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitXor(Xor &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitImplication(Implication &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitEquivalence(Equivalence &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitNot(Not &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitFuture(Future &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitGlobally(Globally &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitHistorically(Historically &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitNextStrong(NextStrong &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitNextWeak(NextWeak &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitOnce(Once &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitPreviously(Previously &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitRelease(Release &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitUntil(Until &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Until>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitSince(Since &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                            int index) {
    std::shared_ptr<Expression> ret;

    // resolving
    if (auto resolving = std::dynamic_pointer_cast<ResolvingOperator>(op.getLeft())) {
        ret = std::make_shared<DerivingOperator>(op.getLeft()->accept(*this, trace, index), trace, index);
    }
    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
        // constant
    } else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
    // predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*predicate));
    }
    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<DerivingOperator>(itr, trace, index);
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }

    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, trace, index);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);

    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) { // TODO check if within interval
            ret = nextWeak->getLeft();
        } else {
            ret = std::make_shared<Constant>(true);
        }
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) { // TODO check if within interval
            ret = nextStrong->getLeft();
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        Or::SetType_T orSet{};
        if (until->getInterval()->containsZero()) {
            auto deriving = std::make_shared<DerivingOperator>(until->getRight(), trace, index);
            orSet.emplace(deriving);
        } else
            orSet.emplace(std::make_shared<Constant>(false));
        if (trace.getEvent(index).isTerminal())
            orSet.emplace(std::make_shared<Constant>(false));
        else {
            And::SetType_T andSet{};
            until->reduceInterval();
            andSet.emplace(until);
            auto derivingExpr = std::make_shared<DerivingOperator>(until->getLeft(), trace, index);
            andSet.emplace(derivingExpr);
            orSet.emplace(std::make_shared<And>(andSet));
        }
        ret = std::make_shared<Or>(orSet);
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        auto resolvingExpr = std::make_shared<ResolvingOperator>(previously, trace, index);
        ret = std::make_shared<DerivingOperator>(resolvingExpr, trace, index);
    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        auto resolvingExpr = ResolvingOperator(since, trace, index);
        ret = std::make_shared<DerivingOperator>(
            DerivingOperator(resolvingExpr.accept(*this, trace, index), trace, index));
    }
    return ret->accept(*this, trace, index);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                             int index) {

    std::shared_ptr<Expression> ret;

    // Deriving
    if (auto deriving = std::dynamic_pointer_cast<DerivingOperator>(op.getLeft())) {
        ret = std::make_shared<ResolvingOperator>(op.getLeft()->accept(*this, trace, index), trace, index);
    }
    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
    } else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto resolvingPastExpr = std::make_shared<ResolvingOperator>(itr, trace, index);
            newExprSet.emplace(resolvingPastExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto resolvingExpr = std::make_shared<ResolvingOperator>(itr, trace, index);
            newExprSet.emplace(resolvingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto resolvingExpr = std::make_shared<ResolvingOperator>(itr, trace, index);
            newExprSet.emplace(resolvingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);

    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(op.getLeft())) {
        ret = nextWeak;
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(op.getLeft())) {
        ret = nextStrong;
    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        ret = until;
    }
    // Previous
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        if (trace.getEvent(index).getTimeStep() == 1) // TODO check if within interval
            ret = std::make_shared<Constant>(false);
        else {
            auto derivingExpr = std::make_shared<DerivingOperator>(previously->getLeft(), trace, index - 1);
            ret = std::make_shared<ResolvingOperator>(derivingExpr, trace, index);
        }
    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        Or::SetType_T orSet{};
        if (since->getInterval()->containsZero()) {
            auto resolving = std::make_shared<ResolvingOperator>(since->getRight(), trace, index);
            orSet.emplace(resolving);
        } else
            orSet.emplace(std::make_shared<Constant>(false));
        if (trace.getEvent(index).getTimeStep() == 0)
            orSet.emplace(std::make_shared<Constant>(false));
        else {
            And::SetType_T andSet{};
            since->reduceInterval();
            auto derivingExpr = DerivingOperator(since, trace, index - 1);
            andSet.emplace(derivingExpr.accept(*this, trace, index - 1));
            andSet.emplace(since->getLeft());
            auto resolvingExpr = std::make_shared<ResolvingOperator>(std::make_shared<And>(andSet), trace, index);
            orSet.emplace(resolvingExpr);
        }
        ret = std::make_shared<Or>(orSet);
    }
    return ret->accept(*this, trace, index);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitEventOperator(EventOperator &op, Trace &trace, int index) {
    return std::make_shared<EventOperator>(op);
}