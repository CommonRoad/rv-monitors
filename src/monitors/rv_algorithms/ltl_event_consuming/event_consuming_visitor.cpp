#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming_visitor.h"
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

void EventConsumingVisitor::consumeEvent(Formula &formula, const std::shared_ptr<Trace> &trace, size_t index) {
    std::shared_ptr<EventOperator> eventOperator = std::make_shared<EventOperator>(formula.getExpr(), trace, index);
    formula.setExpr(eventOperator->accept(*this, trace, index));
}

std::shared_ptr<Expression> EventConsumingVisitor::visitAtomic(Atomic &opr, const std::shared_ptr<Trace> &trace,
                                                               size_t index) {
    return std::make_shared<Atomic>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitPredicate(Predicate &opr, const std::shared_ptr<Trace> &trace,
                                                                  size_t index) {
    return std::make_shared<Predicate>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitConstant(Constant &opr, const std::shared_ptr<Trace> &trace,
                                                                 size_t index) {
    return std::make_shared<Constant>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitAnd(And &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<And>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitOr(Or &opr, const std::shared_ptr<Trace> &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Or>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitXor(Xor &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Xor>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitImplication(Implication &opr,
                                                                    const std::shared_ptr<Trace> &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Implication>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitEquivalence(Equivalence &opr,
                                                                    const std::shared_ptr<Trace> &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Equivalence>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNot(Not &opr, const std::shared_ptr<Trace> &trace,
                                                            size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Not>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitFuture(Future &opr, const std::shared_ptr<Trace> &trace,
                                                               size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Future>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitGlobally(Globally &opr, const std::shared_ptr<Trace> &trace,
                                                                 size_t index) {
    visitExpression(opr, trace, index);
    visitExpression(opr, trace, index); // TODO why two times
    return std::make_shared<Globally>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNextStrong(NextStrong &opr, const std::shared_ptr<Trace> &trace,
                                                                   size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextStrong>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitNextWeak(NextWeak &opr, const std::shared_ptr<Trace> &trace,
                                                                 size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextWeak>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitOnce(Once &opr, const std::shared_ptr<Trace> &trace,
                                                             size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Once>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitPreviously(Previously &opr, const std::shared_ptr<Trace> &trace,
                                                                   size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Previously>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitRelease(Release &opr, const std::shared_ptr<Trace> &trace,
                                                                size_t index) {
    visitExpression(opr, trace, index);
    auto ret = std::make_shared<Release>(opr);
    visitExpression(opr, trace, index);
    return ret;
}

std::shared_ptr<Expression> EventConsumingVisitor::visitUntil(Until &opr, const std::shared_ptr<Trace> &trace,
                                                              size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Until>(opr.getLeft(), opr.getRight());
}

std::shared_ptr<Expression> EventConsumingVisitor::visitSince(Since &opr, const std::shared_ptr<Trace> &trace,
                                                              size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<Since>(opr.getLeft(), opr.getRight(), opr.getInterval());
}

std::shared_ptr<Expression>
EventConsumingVisitor::visitHistorically(Historically &opr, const std::shared_ptr<Trace> &trace, size_t index) {
    visitExpression(opr, trace, index);
    visitExpression(opr, trace, index);
    return std::make_shared<Historically>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitResolvingOperator(ResolvingOperator &opr,
                                                                          const std::shared_ptr<Trace> &trace,
                                                                          size_t index) {
    return std::make_shared<ResolvingOperator>(opr);
}

std::shared_ptr<Expression>
EventConsumingVisitor::visitDerivingOperator(DerivingOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) {
    return std::make_shared<DerivingOperator>(opr);
}

std::shared_ptr<Expression>
EventConsumingVisitor::visitEventOperator(EventOperator &opr, const std::shared_ptr<Trace> &trace, size_t index) {

    std::shared_ptr<Expression> ret;

    // Apply eventMap to atomic
    if (auto atomic = std::dynamic_pointer_cast<Atomic>(opr.getLeft())) {
        ret = std::make_shared<Constant>(trace->getEvent(index).getValue(*atomic));
        // Constant
    } else if (auto constant = std::dynamic_pointer_cast<Constant>(opr.getLeft())) {
        ret = std::make_shared<Constant>(constant->getValue());
    }
    // predicate
    else if (auto predicate = std::dynamic_pointer_cast<Predicate>(opr.getLeft())) {
        ret = std::make_shared<Constant>(trace->getEvent(index).getValue(*predicate));
    }
    // And
    else if (auto andExpr = std::dynamic_pointer_cast<And>(opr.getLeft())) {
        And::SetType_T newExprSet;
        for (const auto &itr : *andExpr) {
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<And>(newExprSet);
    }
    // Or
    else if (auto orExpr = std::dynamic_pointer_cast<Or>(opr.getLeft())) {
        Or::SetType_T newExprSet;
        for (const auto &itr : *orExpr) {
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<Or>(newExprSet);
    }
    // Xor
    else if (auto xorExpr = std::dynamic_pointer_cast<Xor>(opr.getLeft())) {
        Xor::SetType_T newExprSet;
        for (const auto &itr : *xorExpr) {
            auto eventConsumingExpr = std::make_shared<EventOperator>(itr, trace, index);
            newExprSet.emplace(eventConsumingExpr);
        }
        ret = std::make_shared<Xor>(newExprSet);
    }
    // Globally
    else if (auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
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
    else if (auto nextWeak = std::dynamic_pointer_cast<NextWeak>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
            ret = nextWeak->getLeft();
        } else {
            ret = std::make_shared<Constant>(true);
        }
    }
    // Strong Next
    else if (auto nextStrong = std::dynamic_pointer_cast<NextStrong>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
            ret = nextStrong->getLeft();
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Future
    else if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
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
    else if (auto until = std::dynamic_pointer_cast<Until>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
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
    else if (auto previously = std::dynamic_pointer_cast<Previously>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
            ret = previously->getLeft();
        } else {
            ret = std::make_shared<Constant>(false);
        }
    }
    // Once
    else if (auto once = std::dynamic_pointer_cast<Once>(opr.getLeft())) {
        if (!trace->getEvent(index).isTerminal()) {
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

std::shared_ptr<Expression> EventConsumingVisitor::visitUniversalQuantifier(UniversalQuantifier &opr,
                                                                            const std::shared_ptr<Trace> &trace,
                                                                            size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<UniversalQuantifier>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitExistentialQuantifier(ExistentialQuantifier &opr,
                                                                              const std::shared_ptr<Trace> &trace,
                                                                              size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<ExistentialQuantifier>(opr);
}

std::shared_ptr<Expression>
EventConsumingVisitor::visitQuoteQuantifier(QuoteQuantifier &opr, const std::shared_ptr<Trace> &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<QuoteQuantifier>(opr);
}

std::shared_ptr<Expression> EventConsumingVisitor::visitCountingQuantifier(CountingQuantifier &opr,
                                                                           const std::shared_ptr<Trace> &trace,
                                                                           size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<CountingQuantifier>(opr);
}
