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

void MTLDerivingFutureVisitor::consumeEvent(Formula &formula, Trace &trace) {
    std::shared_ptr<ResolvingOperator> resolvingOperator =
            std::make_shared<ResolvingOperator>(formula.getExpr(), trace);
    formula.setExpr(resolvingOperator->accept(*this, trace));
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitAtomic(Atomic &op, Trace &trace) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitConstant(Constant &op, Trace &trace) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitAnd(And &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<And>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitOr(Or &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Or>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitXor(Xor &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Xor>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitImplication(Implication &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Implication>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitEquivalence(Equivalence &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Equivalence>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNot(Not &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitFuture(Future &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitGlobally(Globally &op, Trace &trace) {
    visitExpression(op, trace);
    visitExpression(op, trace);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNextStrong(NextStrong &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitNextWeak(NextWeak &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitOnce(Once &op, Trace &trace) {
    visitExpression(op, trace);
    auto ret = std::make_shared<Once>(op);
    visitExpression(op, trace);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitPreviously(Previously &op, Trace &trace) {
    visitExpression(op, trace);
    auto ret = std::make_shared<Previously>(op);
    visitExpression(op, trace);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitRelease(Release &op, Trace &trace) {
    visitExpression(op, trace);
    auto ret = std::make_shared<Release>(op);
    visitExpression(op, trace);
    return ret;
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitUntil(Until &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Until>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitSince(Since &op, Trace &trace) {
    visitExpression(op, trace);
    return std::make_shared<Since>(op.getLeft(), op.getRight());
}

std::shared_ptr<Expression>
MTLDerivingFutureVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace) {
    std::shared_ptr<Expression> ret;

    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(op.getLeft())) {
        ret = std::make_shared<Constant>(trace.getLastEvent().getValue(*atomic));
    }
    else if (auto constant = std::dynamic_pointer_cast<Constant>(op.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
        // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(op.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto derivingFutureExpr = std::make_shared<DerivingOperator>(itr, trace);
            newExprSet.emplace(derivingFutureExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }

        // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(op.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, trace);
            newExprSet.emplace(derivingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
        // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(op.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto derivingExpr = std::make_shared<DerivingOperator>(itr, trace);
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
        if (trace.getLastEvent().getTimeStep() == 1) // TODO check if within interval
            ret = std::make_shared<Constant>(false);
        else{
            auto resolvingExpr = std::make_shared<ResolvingOperator>(previously->getLeft(), trace);
            ret = std::make_shared<DerivingOperator>(resolvingExpr, trace);
        }
    }
        // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        Or::SetType_T orSet{};
        if (since->getInterval()->containsZero()) {
            auto derivingExpr = std::make_shared<DerivingOperator>(since->getRight(), trace);
            orSet.emplace(derivingExpr);
        }
        else
            orSet.emplace(std::make_shared<Constant>(false));
        if (trace.getLastEvent().getTimeStep() == 1)
            orSet.emplace(std::make_shared<Constant>(false));
        else{

        }
        ret = std::make_shared<Or>(orSet);
    }
    return ret->accept(*this, trace);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> MTLDerivingFutureVisitor::visitEventOperator(EventOperator &op, Trace &trace) {
    return std::make_shared<EventOperator>(op);
}