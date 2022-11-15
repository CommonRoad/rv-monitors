#include "event_consuming_visitor.h"
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

void EventConsumingVisitor::consumeEvent(Formula &formula, Trace &trace, size_t index) {
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> EventConsumingVisitor::visitAtomic(Atomic &op, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitPredicate(Predicate &op, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitConstant(Constant &op, Trace &trace, size_t index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitAnd(And &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitOr(Or &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitXor(Xor &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitImplication(Implication &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitEquivalence(Equivalence &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNot(Not &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitFuture(Future &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitGlobally(Globally &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index); // TODO why two times
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNextStrong(NextStrong &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNextWeak(NextWeak &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitOnce(Once &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitPreviously(Previously &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitRelease(Release &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitUntil(Until &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Until>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression> EventConsumingVisitor::visitSince(Since &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> EventConsumingVisitor::visitHistorically(Historically &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                          size_t index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                         size_t index) {
    return std::make_shared<DerivingOperator>(op);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitEventOperator(EventOperator &op, Trace &trace, size_t index) {

    std::shared_ptr<Expression> ret;

    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getEvent(index).getValue(*atomic));
        // Constant
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
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            And::SetType_T newSet;
            // Propagating down the expression tree
            auto eventOperatorExpr = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
            newSet.emplace(eventOperatorExpr);
            newSet.emplace(globally);
            ret = std::make_shared<And>(newSet);
        } else {
            ret = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
        }
    }
    // Weak Next
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            ret = nextWeak->getLeft();
        } else {
            ret = std::make_shared<Constant>(true);
        }
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            ret = nextStrong->getLeft();
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            Or::SetType_T newSet;
            auto eventOperatorExpr = std::make_shared<EventOperator>(future->getLeft(), trace, index);
            newSet.emplace(eventOperatorExpr);
            newSet.emplace(future);
            ret = std::make_shared<Or>(newSet);
        } else {
            ret = std::make_shared<EventOperator>(future->getLeft(), trace, index);
        }
    }
    // Until
    else if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            Or::SetType_T newSetOr;
            And::SetType_T newSetAnd;
            auto orElement = std::make_shared<EventOperator>(until->getRight(), trace, index);
            auto andElement = std::make_shared<EventOperator>(until->getLeft(), trace, index);
            newSetOr.emplace(orElement);
            newSetAnd.emplace(andElement);
            newSetAnd.emplace(until);
            auto newAnd = std::make_shared<And>(newSetAnd);
            newSetOr.emplace(newAnd);
            ret = std::make_shared<Or>(newSetOr);
        } else {
            ret = std::make_shared<EventOperator>(until->getRight(), trace, index);
        }
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            ret = previously->getLeft();
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal()) {
            Or::SetType_T newSet;
            auto eventOperatorExpr = std::make_shared<EventOperator>(once->getLeft(), trace, index);
            newSet.emplace(eventOperatorExpr);
            newSet.emplace(once);
            ret = std::make_shared<Or>(newSet);
        } else {
            ret = std::make_shared<EventOperator>(once->getLeft(), trace, index);
        }
    }
    return ret->accept(*this, trace, index);
}
