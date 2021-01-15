#include "MTLDerivingFutureVisitor.h"
#include "ExpressionTree/Atomic/Atomic.h"
#include "ExpressionTree/Atomic/Constant.h"
#include "ExpressionTree/Unary/Bool/Not.h"
#include "ExpressionTree/Nary/Bool/And.h"
#include "ExpressionTree/Nary/Bool/Or.h"
#include "ExpressionTree/Nary/Bool/Xor.h"
#include "ExpressionTree/Binary/Bool/Implication.h"
#include "ExpressionTree/Binary/Bool/Equivalence.h"
#include "ExpressionTree/Unary/ResolvingOperator.h"
#include "ExpressionTree/Unary/DerivingOperator.h"
#include "ExpressionTree/Unary/EventOperator.h"
#include "ExpressionTree/Unary/Temporal/LTLf/Globally.h"
#include "ExpressionTree/Unary/Temporal/LTLf/Future.h"
#include "ExpressionTree/Unary/Temporal/LTLf/NextStrong.h"
#include "ExpressionTree/Unary/Temporal/LTLf/NextWeak.h"
#include "ExpressionTree/Unary/Temporal/PLTLf/Once.h"
#include "ExpressionTree/Unary/Temporal/PLTLf/Previously.h"
#include "ExpressionTree/Binary/Temporal/LTLf/Release.h"
#include "ExpressionTree/Binary/Temporal/LTLf/Until.h"
#include "ExpressionTree/Binary/Temporal/PTLf/Since.h"
#include "Trace/Event.h"

void MTLDerivingFutureVisitor::consumeEvent(Formula &formula, Event &event) {
    std::shared_ptr<ResolvingOperator> resolvingOperator =
            std::make_shared<ResolvingOperator>(formula.getExpr(), event);
    formula.setExpr(resolvingOperator->accept(*this, event));
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitAtomic(Atomic &op, Event &event) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitConstant(Constant &op, Event &event) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitAnd(And &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitOr(Or &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitXor(Xor &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitImplication(Implication &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitEquivalence(Equivalence &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNot(Not &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitFuture(Future &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitGlobally(Globally &op, Event &event) {
    visitExpression(op, event);
    visitExpression(op, event);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNextStrong(NextStrong &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNextWeak(NextWeak &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitOnce(Once &op, Event &event) {
    visitExpression(op, event);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, event);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitPreviously(Previously &op, Event &event) {
    visitExpression(op, event);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, event);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitRelease(Release &op, Event &event) {
    visitExpression(op, event);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, event);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitUntil(Until &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Until>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitSince(Since &op, Event &event) {
    visitExpression(op, event);
    return std::make_shared<Since>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitDerivingOperator(DerivingOperator &op, Event &event) {
    std::shared_ptr<Expression> ret;

    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(event.getValue(*atomic));
    }
    else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
        // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<DerivingOperator>(itr, event);
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }

        // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, event);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
        // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, event);
            newExprSet.emplace(derivingExpr);
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
        // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        if (event.getTimeStep() == 1)
            ret = std::make_shared<Constant>(false);
        else{
            auto resolvingExpr = std::make_shared<ResolvingOperator>(previously->getLeft(), event);
            ret = std::make_shared<DerivingOperator>(resolvingExpr, event);
        }
    }
        // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        Or::SetType_T orSet{};
        if (since->getInterval()->containsZero()) {
            auto derivingExpr = std::make_shared<DerivingOperator>(since->getRight(), event);
            orSet.emplace(derivingExpr);
        }
        else
            orSet.emplace(std::make_shared<Constant>(false));
        if (event.getTimeStep() == 1)
            orSet.emplace(std::make_shared<Constant>(false));
        else{

        }
        ret = std::make_shared<Or>(orSet);
    }
    return ret->accept(*this, event);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitResolvingOperator(ResolvingOperator &op, Event &event) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitEventOperator(EventOperator &op, Event &event) {
    return std::make_shared<EventOperator>(op);
}