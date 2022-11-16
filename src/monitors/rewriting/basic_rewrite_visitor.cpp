#include "basic_rewrite_visitor.h"
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
#include "monitors/trace/event.h"
#include "rewrite_visitor.h"
#include "rewrite_visitor_utils.h"
#include <iostream>

void BasicRewriteVisitor::rewrite(Formula &formula) {
    event_map_t map;
    // Dummy eventMap
    Event event(std::move(map), 0, false);
    auto trace = Trace(std::vector<Event>{event});

    auto before = Formula::deepCopy(formula.getExpr());
    formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    while (before->to_string() != formula.getExpr()->to_string()) {
        before = Formula::deepCopy(formula.getExpr());
        formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    }
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitAtomic(Atomic &opr, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitConstant(Constant &opr, Trace &trace, size_t index) {
    return std::make_shared<Constant>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitPredicate(Predicate &opr, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitAnd(And &opr, Trace &trace, size_t index) {
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

    return std::make_shared<And>(expressions);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitOr(Or &opr, Trace &trace, size_t index) {
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

    return std::make_shared<Or>(expressions);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitXor(Xor &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);

    auto expressions = opr.getExpressions();
    Xor::SetType_T newXorSet;

    // !a ^ a = true
    RewriteVisitorUtils::reduceNegations<true>(expressions);

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

std::shared_ptr<Expression> BasicRewriteVisitor::visitImplication(Implication &opr, Trace &trace, size_t index) {
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

    // a -> b = !a | b
    Or::SetType_T orSet{std::make_shared<Not>(opr.getLeft()), opr.getRight()};
    return std::make_shared<Or>(orSet);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitNot(Not &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // !true = false
    if (opr.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(false);
    }
    // !false = true
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }
    return std::make_shared<Not>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitFuture(Future &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // F(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // Idempotence
    if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
        return future;
    }

    // Absorption F(G(F(a))) = G(F(a))
    if (auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft())) {
        if (std::dynamic_pointer_cast<Future>(globally->getLeft())) {
            return globally;
        }
    }
    return std::make_shared<Future>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitGlobally(Globally &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // G(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // G(true) = true
    if (opr.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // Idempotence
    if (auto globally = std::dynamic_pointer_cast<Globally>(opr.getLeft())) {
        return globally;
    }

    // Absorption G(F(G(a))) = F(G(a))
    if (auto future = std::dynamic_pointer_cast<Future>(opr.getLeft())) {
        if (std::dynamic_pointer_cast<Globally>(future->getLeft())) {
            return future;
        }
    }
    return std::make_shared<Globally>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitHistorically(Historically &opr, Trace &trace, size_t index) {
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
    return std::make_shared<Historically>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitNextStrong(NextStrong &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // X(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    return std::make_shared<NextStrong>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitNextWeak(NextWeak &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    return std::make_shared<NextWeak>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitOnce(Once &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // O(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto once = std::dynamic_pointer_cast<Once>(opr.getLeft())) {
        return once;
    }
    return std::make_shared<Once>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitPreviously(Previously &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // P(false) = false
    if (opr.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    return std::make_shared<Previously>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitRelease(Release &opr, Trace &trace, size_t index) {
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
    return std::make_shared<Release>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitUntil(Until &opr, Trace &trace, size_t index) {
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
    return std::make_shared<Until>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitSince(Since &opr, Trace &trace, size_t index) {
    visitExpression(opr, trace, index);
    // a S false = false
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
    return std::make_shared<Since>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitEventOperator(EventOperator &opr, Trace &trace, size_t index) {
    return std::make_shared<EventOperator>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitResolvingOperator(ResolvingOperator &opr, Trace &trace,
                                                                        size_t index) {
    return std::make_shared<ResolvingOperator>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitDerivingOperator(DerivingOperator &opr, Trace &trace,
                                                                       size_t index) {
    return std::make_shared<DerivingOperator>(opr);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitEquivalence(Equivalence &opr, Trace &trace, size_t index) {
    // TODO check if same/different formula left and right
    if (opr.getLeft() == opr.getRight()) {
        return std::make_shared<Constant>(true);
    }
    // a -> b & b -> a
    auto newLeft = std::make_shared<Implication>(opr.getLeft(), opr.getRight());
    auto newRight = std::make_shared<Implication>(opr.getRight(), opr.getLeft());
    And::SetType_T andSet{newLeft, newRight};
    return std::make_shared<And>(andSet);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitUniversalQuantifier(UniversalQuantifier &op, Trace &trace,
                                                                          size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<UniversalQuantifier>(op);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitExistentialQuantifier(ExistentialQuantifier &op, Trace &trace,
                                                                            size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<ExistentialQuantifier>(op);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitQuoteQuantifier(QuoteQuantifier &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<QuoteQuantifier>(op);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitCountingQuantifier(CountingQuantifier &op, Trace &trace,
                                                                         size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<CountingQuantifier>(op);
}

std::shared_ptr<Expression> BasicRewriteVisitor::visitEqualityOperator(EqualityOperator &op, Trace &trace,
                                                                       size_t index) {
    return std::make_shared<EqualityOperator>(op);
}
