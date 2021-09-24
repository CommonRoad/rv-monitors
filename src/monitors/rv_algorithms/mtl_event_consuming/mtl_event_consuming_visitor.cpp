#include "mtl_event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
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

void MTLEventConsumingVisitor::consumeEvent(Formula &formula, Trace &trace, size_t index) {
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitAtomic(Atomic &op, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(op);
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
    visitExpression(op, trace, index);
    visitExpression(op, trace, index);
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> MTLEventConsumingVisitor::visitHistorically(Historically &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
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
    return std::make_shared<Since>(op.getLeft(), op.getRight(), op.getInterval());
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
        std::cout << "TODO \n"; // TODO
    }
    // Since
    else if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        std::cout << "TODO \n"; // TODO
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal() and globally->getInterval()->getEnd() != 0) {
            And::SetType_T newSet;
            globally->reduceInterval();
            auto eventOperatorExpr = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
            newSet.emplace(eventOperatorExpr);
            newSet.emplace(globally);
            ret = std::make_shared<And>(newSet);
        } else {
            ret = std::make_shared<EventOperator>(globally->getLeft(), trace, index);
        }
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
        if (!trace.getEvent(index).isTerminal() and future->getInterval()->getEnd() != 0) {
            Or::SetType_T newSet;
            auto eventOperatorExpr = std::make_shared<EventOperator>(future->getLeft(), trace, index);
            newSet.emplace(eventOperatorExpr);
            future->reduceInterval();
            newSet.emplace(future);
            ret = std::make_shared<Or>(newSet);
        } else {
            ret = std::make_shared<EventOperator>(future->getLeft(), trace, index);
        }
    }
    // Previously
    else if (auto previously = std::dynamic_pointer_cast<Previously>(op.getLeft())) {
        if (index > 0) {
            auto eventOperatorExpr = std::make_shared<EventOperator>(previously->getLeft(), trace, index - 1);
            ret = eventOperatorExpr->accept(*this, trace, index - 1);
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        if (once->getInterval()->getEnd() != 0 and index > 0) {
            if (once->getInterval()->getStart() > 0) {
                once->reduceInterval();
                auto his = std::make_shared<EventOperator>(once, trace, index - 1); // evtl. once or newOnce ???
                ret = his->accept(*this, trace, index - 1);
            } else {
                Or::SetType_T newSet;
                auto eventOperatorExpr = std::make_shared<EventOperator>(once->getLeft(), trace, index);
                newSet.emplace(eventOperatorExpr);
                Once newOnce = *once;
                newOnce.reduceInterval();
                auto his = std::make_shared<EventOperator>(std::make_shared<Once>(newOnce), trace,
                                                           index - 1); // evtl. once or newOnce ???
                auto his_resolved = his->accept(*this, trace, index - 1);
                newSet.emplace(his_resolved);
                ret = std::make_shared<Or>(newSet);
            }
        } else {
            if (once->getInterval()->getStart() > 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(once->getLeft(), trace, index);
        }
    }
    // Historically
    else if (auto historically = std::dynamic_pointer_cast<Historically>(op.getLeft())) {
        if (historically->getInterval()->getEnd() != 0 and index > 0) {
            if (historically->getInterval()->getStart() > 0) {
                historically->reduceInterval();
                auto eventOperatorExpr =
                    std::make_shared<EventOperator>(std::make_shared<Historically>(historically), trace, index - 1);
                ret = eventOperatorExpr->accept(*this, trace, index - 1);
            } else {
                And::SetType_T newSet;
                Historically newHistorically = *historically;
                newHistorically.reduceInterval();
                auto eventOperatorExpr =
                    std::make_shared<EventOperator>(std::make_shared<Historically>(newHistorically), trace, index - 1);
                auto sol = eventOperatorExpr->accept(*this, trace, index - 1);
                newSet.emplace(sol);
                newSet.emplace(std::make_shared<EventOperator>(historically->getLeft(), trace, index));
                ret = std::make_shared<And>(newSet);
            }
        } else {
            if (historically->getInterval()->getStart() > 0)
                ret = std::make_shared<Constant>(false);
            else
                ret = std::make_shared<EventOperator>(historically->getLeft(), trace, index);
        }
    }
    return ret->accept(*this, trace, index);
}
std::shared_ptr<Expression> MTLEventConsumingVisitor::visitPredicate(Predicate &op, Trace &trace, size_t index) {
    return std::shared_ptr<Predicate>();
}
