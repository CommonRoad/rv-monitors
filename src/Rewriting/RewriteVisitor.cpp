#include "RewriteVisitor.h"
#include "ExpressionTree/Atomic/Atomic.h"
#include "ExpressionTree/Atomic/Constant.h"
#include "ExpressionTree/Binary/Binary.h"
#include "ExpressionTree/Binary/Bool/Equivalence.h"
#include "ExpressionTree/Binary/Bool/Implication.h"
#include "ExpressionTree/Binary/Temporal/LTLf/Release.h"
#include "ExpressionTree/Binary/Temporal/LTLf/Until.h"
#include "ExpressionTree/Binary/Temporal/PTLf/Since.h"
#include "ExpressionTree/Nary/Bool/And.h"
#include "ExpressionTree/Nary/Bool/Or.h"
#include "ExpressionTree/Nary/Bool/Xor.h"
#include "ExpressionTree/Nary/NaryInterface.h"
#include "ExpressionTree/Unary/Bool/Not.h"
#include "ExpressionTree/Unary/DerivingOperator.h"
#include "ExpressionTree/Unary/EventOperator.h"
#include "ExpressionTree/Unary/ResolvingOperator.h"
#include "ExpressionTree/Unary/Temporal/LTLf/Future.h"
#include "ExpressionTree/Unary/Temporal/LTLf/Globally.h"
#include "ExpressionTree/Unary/Temporal/LTLf/NextStrong.h"
#include "ExpressionTree/Unary/Temporal/LTLf/NextWeak.h"
#include "ExpressionTree/Unary/Temporal/PLTLf/Historically.h"
#include "ExpressionTree/Unary/Temporal/PLTLf/Once.h"
#include "ExpressionTree/Unary/Temporal/PLTLf/Previously.h"
#include "ExpressionTree/Unary/Unary.h"
#include "RewriteVisitorUtils.h"
#include "Trace/Event.h"
#include <iostream>

void RewriteVisitor::rewrite(Formula &formula) {
    std::map<std::string, LazyBool> map;
    // Dummy eventMap
    Event event(map, 0, false);
    auto trace = Trace(std::vector<Event>{event});

    auto before = Formula::deepCopy(formula.getExpr());
    formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    while (!Formula::sameTree(before, formula.getExpr())) {
        before = Formula::deepCopy(formula.getExpr());
        formula.setExpr(formula.getExpr()->accept(*this, trace, 0));
    }
}

std::shared_ptr<Expression> RewriteVisitor::visitAtomic(Atomic &op, Trace &trace, int index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitConstant(Constant &op, Trace &trace, int index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitAnd(And &op, Trace &trace, int index) {
    visitExpression(op, trace, index);

    // I'm a copy!
    auto expressions = op.getExpressions();

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

    // a & (b ^ c) = a & b ^ a & c
    for (auto itr = expressions.begin(); itr != expressions.end(); ++itr) {
        for (auto itrInner = expressions.begin(); itrInner != itr && itrInner != expressions.end(); ++itrInner) {
            // Found an Xor
            if (auto xorExpr = std::dynamic_pointer_cast<Xor>(*itrInner)) {
                Xor::SetType_T newXorExpr;
                for (const auto &exp : xorExpr->getExpressions()) {
                    // Create new Xor according to rule
                    newXorExpr.emplace(std::make_shared<And>(And::SetType_T({*itr, exp})));
                }
                // Add newXor to expressions
                expressions.emplace(std::make_shared<Xor>(newXorExpr));
                // Erase Xor and itr
                itrInner = expressions.erase(itrInner);
                itr = expressions.erase(itr);
                break;
            }
        }
    }

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    return std::make_shared<And>(expressions);
}

std::shared_ptr<Expression> RewriteVisitor::visitOr(Or &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    auto expressions = op.getExpressions();

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

    // a | b = a & b ^ a ^ b
    And::SetType_T andSet;
    Xor::SetType_T xorSet;

    auto iter = expressions.begin();
    andSet.emplace(*iter);
    xorSet.emplace(*iter++);
    andSet.emplace(*iter);
    xorSet.emplace(*iter);

    // Remove a and b
    expressions.erase(expressions.begin());
    expressions.erase(expressions.begin());

    auto andExpr = std::make_shared<And>(andSet);
    xorSet.emplace(andExpr);
    auto xorExpr = std::make_shared<Xor>(xorSet);

    // At least one element left
    if (!expressions.empty()) {
        expressions.emplace(xorExpr);
        return std::make_shared<Or>(expressions);
    }

    return xorExpr;
}

std::shared_ptr<Expression> RewriteVisitor::visitXor(Xor &op, Trace &trace, int index) {
    visitExpression(op, trace, index);

    auto expressions = op.getExpressions();

    // !a ^ a = true
    RewriteVisitorUtils::reduceNegations<true>(expressions);

    Xor::SetType_T newXorSet;

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

std::shared_ptr<Expression> RewriteVisitor::visitImplication(Implication &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // a -> a = true
    if (*op.getLeft() == *op.getRight()) {
        return std::make_shared<Constant>(true);
    }
    // true -> false = false
    if (op.getLeft()->isConstTrue() && op.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // a -> true = true
    if (op.getRight()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // false -> a = true
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }
    // a -> b = true ^ a ^ a & b
    And::SetType_T andSet{op.getLeft(), op.getRight()};
    auto andExpr = std::make_shared<And>(andSet);
    Xor::SetType_T xorSet{std::make_shared<Constant>(true), op.getLeft(), andExpr};
    return std::make_shared<Xor>(xorSet);
}

std::shared_ptr<Expression> RewriteVisitor::visitEquivalence(Equivalence &op, Trace &trace, int index) {
    // TODO check if same/different formula left and right
    if (op.getLeft() == op.getRight()) {
        return std::make_shared<Constant>(true);
    }

    // a <-> b = true ^ a ^ b
    auto trueExpr = std::make_shared<Constant>(true);
    Xor::SetType_T xorSet{op.getLeft(), op.getRight(), trueExpr};
    return std::make_shared<Xor>(xorSet);
}

std::shared_ptr<Expression> RewriteVisitor::visitNot(Not &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // !true = false
    if (op.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(false);
    }
    // !false = true
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }
    // !a = true ^ a
    Xor::SetType_T xorSet{std::make_shared<Constant>(true), op.getLeft()};
    return std::make_shared<Xor>(xorSet);
}

std::shared_ptr<Expression> RewriteVisitor::visitFuture(Future &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // F(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // Idempotence F(F(a)) = F(a)
    if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
        return future;
    }

    // Absorption F(G(F(a))) = G(F(a))
    if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        if (std::dynamic_pointer_cast<Future>(globally->getLeft())) {
            return globally;
        }
    }
    return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitGlobally(Globally &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // G(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // G(true) = true
    if (op.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // Idempotence
    if (auto globally = std::dynamic_pointer_cast<Globally>(op.getLeft())) {
        return globally;
    }
    // Absorption G(F(G(a))) = F(G(a))
    if (auto future = std::dynamic_pointer_cast<Future>(op.getLeft())) {
        if (std::dynamic_pointer_cast<Globally>(future->getLeft())) {
            return future;
        }
    }
    return std::make_shared<Globally>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitHistorically(Historically &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // H(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    // H(true) = true
    if (op.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // Idempotence
    if (auto historically = std::dynamic_pointer_cast<Historically>(op.getLeft())) {
        return historically;
    }
    // Absorption H(O(H(a))) = O(H(a))
    if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        if (std::dynamic_pointer_cast<Historically>(once->getLeft())) {
            return once;
        }
    }
    return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitNextStrong(NextStrong &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // X(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitNextWeak(NextWeak &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitOnce(Once &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // O(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        return once;
    }
    return std::make_shared<Once>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitPreviously(Previously &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    return std::make_shared<Previously>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitRelease(Release &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // a R true = true
    if (op.getRight()->isConstTrue()) {
        return std::make_shared<Constant>(true);
    }
    // a R false = false
    if (op.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto release = std::dynamic_pointer_cast<Release>(op.getLeft())) {
        if (release->getRight() == op.getRight()) {
            return release;
        }
    }
    if (auto release = std::dynamic_pointer_cast<Release>(op.getRight())) {
        if (release->getLeft() == op.getLeft()) {
            return release;
        }
    }
    return std::make_shared<Release>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitUntil(Until &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // a U false = false
    if (op.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto until = std::dynamic_pointer_cast<Until>(op.getLeft())) {
        if (until->getRight() == op.getRight()) {
            return until;
        }
    }
    if (auto until = std::dynamic_pointer_cast<Until>(op.getRight())) {
        if (until->getLeft() == op.getLeft()) {
            return until;
        }
    }
    return std::make_shared<Until>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitSince(Since &op, Trace &trace, int index) {
    visitExpression(op, trace, index);
    // a U false = false
    if (op.getRight()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto since = std::dynamic_pointer_cast<Since>(op.getLeft())) {
        if (since->getRight() == op.getRight()) {
            return since;
        }
    }
    if (auto since = std::dynamic_pointer_cast<Since>(op.getRight())) {
        if (since->getLeft() == op.getLeft()) {
            return since;
        }
    }
    return std::make_shared<Since>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitEventOperator(EventOperator &op, Trace &trace, int index) {
    return std::make_shared<EventOperator>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace, int index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> RewriteVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace, int index) {
    return std::make_shared<DerivingOperator>(op);
}
