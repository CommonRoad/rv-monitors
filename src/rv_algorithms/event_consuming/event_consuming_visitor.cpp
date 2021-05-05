#include "event_consuming_visitor.h"
#include "expression_tree/atomic/atomic.h"
#include "expression_tree/atomic/constant.h"
#include "expression_tree/binary/bool/equivalence.h"
#include "expression_tree/binary/bool/implication.h"
#include "expression_tree/binary/temporal/ltlf/release.h"
#include "expression_tree/binary/temporal/ltlf/until.h"
#include "expression_tree/binary/temporal/ptlf/since.h"
#include "expression_tree/nary/bool/and.h"
#include "expression_tree/nary/bool/or.h"
#include "expression_tree/nary/bool/xor.h"
#include "expression_tree/unary/bool/not.h"
#include "expression_tree/unary/deriving_operator.h"
#include "expression_tree/unary/event_operator.h"
#include "expression_tree/unary/resolving_operator.h"
#include "expression_tree/unary/temporal/ltlf/future.h"
#include "expression_tree/unary/temporal/ltlf/globally.h"
#include "expression_tree/unary/temporal/ltlf/next_strong.h"
#include "expression_tree/unary/temporal/ltlf/next_weak.h"
#include "expression_tree/unary/temporal/ptlf/historically.h"
#include "expression_tree/unary/temporal/ptlf/once.h"
#include "expression_tree/unary/temporal/ptlf/previously.h"

void event_consuming_visitor::consumeEvent(Formula &formula, Trace &trace, int index) {
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> event_consuming_visitor::visitAtomic(Atomic &op, Trace &trace, int index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitConstant(Constant &op, Trace &trace, int index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitAnd(And &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitOr(Or &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitXor(Xor &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitImplication(Implication &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitEquivalence(Equivalence &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitNot(Not &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitFuture(Future &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitGlobally(Globally &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index); // TODO why two times
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitNextStrong(NextStrong &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitNextWeak(NextWeak &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitOnce(Once &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> event_consuming_visitor::visitPreviously(Previously &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> event_consuming_visitor::visitRelease(Release &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace, index);
    return ret;
}

std::shared_ptr<Expression> event_consuming_visitor::visitUntil(Until &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Until>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression> event_consuming_visitor::visitSince(Since &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
}

std::shared_ptr<Expression> event_consuming_visitor::visitHistorically(Historically &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                          int index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                         int index) {
    return std::make_shared<DerivingOperator>(op);
}

std::shared_ptr<Expression> event_consuming_visitor::visitEventOperator(EventOperator &op, Trace &trace, int index) {

    std::shared_ptr<Expression> ret;

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
            ret = std::make_shared<Constant>(true);
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
