#include "hydra_rewrite_visitor.h"
#include "monitors/expression_tree/atomic/atomic.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/binary.h"
#include "monitors/expression_tree/binary/bool/equivalence.h"
#include "monitors/expression_tree/binary/bool/implication.h"
#include "monitors/expression_tree/binary/temporal/release.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/nary/bool/xor.h"
#include "monitors/expression_tree/nary/nary_interface.h"
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
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/rewriting/rewrite_visitor_utils.h"
#include "monitors/trace/event.h"
#include <iostream>

void HydraRewriteVisitor::rewrite(Formula &formula) {
    event_map_t map;
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

std::shared_ptr<Expression> HydraRewriteVisitor::visitAtomic(Atomic &op, Trace &trace, size_t index) {
    return std::make_shared<Atomic>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitConstant(Constant &op, Trace &trace, size_t index) {
    return std::make_shared<Constant>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitAnd(And &op, Trace &trace, size_t index) {
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

    //    // a & (b ^ c) = a & b ^ a & c
    //    for (auto itr = expressions.begin(); itr != expressions.end(); ++itr) {
    //        for (auto itrInner = expressions.begin(); itrInner != itr && itrInner != expressions.end(); ++itrInner) {
    //            // Found an Xor
    //            if (auto xorExpr = std::dynamic_pointer_cast<Xor>(*itrInner)) {
    //                Xor::SetType_T newXorExpr;
    //                for (const auto &exp : xorExpr->getExpressions()) {
    //                    // Create new Xor according to rule
    //                    newXorExpr.emplace(std::make_shared<And>(And::SetType_T({*itr, exp})));
    //                }
    //                // Add newXor to expressions
    //                expressions.emplace(std::make_shared<Xor>(newXorExpr));
    //                // Erase Xor and itr
    //                itrInner = expressions.erase(itrInner);
    //                itr = expressions.erase(itr);
    //                break;
    //            }
    //        }
    //    }

    // Only one element left
    if (expressions.size() == 1) {
        return *expressions.begin();
    }

    return std::make_shared<And>(expressions);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitOr(Or &op, Trace &trace, size_t index) {
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
    //    And::SetType_T andSet;
    //    Xor::SetType_T xorSet;
    //
    //    auto iter = expressions.begin();
    //    andSet.emplace(*iter);
    //    xorSet.emplace(*iter++);
    //    andSet.emplace(*iter);
    //    xorSet.emplace(*iter);
    //
    //    // Remove a and b
    //    expressions.erase(expressions.begin());
    //    expressions.erase(expressions.begin());
    //
    //    auto andExpr = std::make_shared<And>(andSet);
    //    xorSet.emplace(andExpr);
    //    auto xorExpr = std::make_shared<Xor>(xorSet);

    // At least one element left
    //    if (!expressions.empty()) {
    //        expressions.emplace(xorExpr);
    //        return std::make_shared<Or>(expressions);
    //    }

    //    return xorExpr;
    return std::make_shared<Or>(expressions);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitXor(Xor &op, Trace &trace, size_t index) {
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

std::shared_ptr<Expression> HydraRewriteVisitor::visitImplication(Implication &op, Trace &trace, size_t index) {
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
    //    // a -> b = true ^ a ^ a & b
    //    And::SetType_T andSet{op.getLeft(), op.getRight()};
    //    auto andExpr = std::make_shared<And>(andSet);
    //    Xor::SetType_T xorSet{std::make_shared<Constant>(true), op.getLeft(), andExpr};
    //    return std::make_shared<Xor>(xorSet);

    // a -> b = a v !b
    auto negExpr{std::make_shared<Not>(op.getLeft())};
    Or::SetType_T orSet{op.getRight(), negExpr};
    auto orExpr{std::make_shared<Or>(orSet)};
    return orExpr;
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitEquivalence(Equivalence &op, Trace &trace, size_t index) {
    // TODO check if same/different formula left and right
    if (op.getLeft() == op.getRight()) {
        return std::make_shared<Constant>(true);
    }

    // a <-> b = true ^ a ^ b
    auto trueExpr = std::make_shared<Constant>(true);
    Xor::SetType_T xorSet{op.getLeft(), op.getRight(), trueExpr};
    return std::make_shared<Xor>(xorSet);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitNot(Not &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    // !true = false
    if (op.getLeft()->isConstTrue()) {
        return std::make_shared<Constant>(false);
    }
    // !false = true
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(true);
    }
    //    // !a = true ^ a
    //    Xor::SetType_T xorSet{std::make_shared<Constant>(true), op.getLeft()};
    //    return std::make_shared<Xor>(xorSet);
    return std::make_shared<Not>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitFuture(Future &op, Trace &trace, size_t index) {
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

    // F(a) = true U a
    Until until{std::make_shared<Constant>(true), op.getLeft(), op.getInterval()};
    return std::make_shared<Until>(until);
    //   return std::make_shared<Future>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitGlobally(Globally &op, Trace &trace, size_t index) {
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

    // G(a) = not F(not a) = true ^ F(true ^ a)
    //    Xor::SetType_T xorSetOne{std::make_shared<Constant>(true), op.getLeft()};
    //    auto future {std::make_shared<Future>(std::make_shared<Xor>(xorSetOne), op.getInterval())};
    //    Xor::SetType_T xorSetTwo{std::make_shared<Constant>(true), future};
    //
    //    return std::make_shared<Xor>(xorSetTwo);

    // G(a) = not F(not a) = ! (true U ! a)
    auto future{std::make_shared<Future>(std::make_shared<Not>(op.getLeft()), op.getInterval())};
    return std::make_shared<Not>(future);

    //   return std::make_shared<Globally>(op.getLeft(), op.getInterval());
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitHistorically(Historically &op, Trace &trace, size_t index) {
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

    // H(a) = not O(not a) = true ^ O(true ^ a)
    //    Xor::SetType_T xorSetOne{std::make_shared<Constant>(true), op.getLeft()};
    //    auto once {std::make_shared<Once>(std::make_shared<Xor>(xorSetOne), op.getInterval())};
    //    Xor::SetType_T xorSetTwo{std::make_shared<Constant>(true), once};
    //
    //    return std::make_shared<Xor>(xorSetTwo);

    // H(a) = not O(not a)
    auto once{std::make_shared<Once>(std::make_shared<Not>(op.getLeft()), op.getInterval())};
    return std::make_shared<Not>(once);

    //   return std::make_shared<Historically>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitNextStrong(NextStrong &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    // X(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }
    return std::make_shared<NextStrong>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitNextWeak(NextWeak &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<NextWeak>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitOnce(Once &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    // O(false) = false
    if (op.getLeft()->isConstFalse()) {
        return std::make_shared<Constant>(false);
    }

    // Idempotence
    if (auto once = std::dynamic_pointer_cast<Once>(op.getLeft())) {
        return once;
    }

    // O(a) = true S a
    Since since{std::make_shared<Constant>(true), op.getLeft(), op.getInterval()};
    return std::make_shared<Since>(since);
    // return std::make_shared<Once>(op.getLeft(), op.getInterval());
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitPreviously(Previously &op, Trace &trace, size_t index) {
    visitExpression(op, trace, index);
    return std::make_shared<Previously>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitRelease(Release &op, Trace &trace, size_t index) {
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

std::shared_ptr<Expression> HydraRewriteVisitor::visitUntil(Until &op, Trace &trace, size_t index) {
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

std::shared_ptr<Expression> HydraRewriteVisitor::visitSince(Since &op, Trace &trace, size_t index) {
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

std::shared_ptr<Expression> HydraRewriteVisitor::visitEventOperator(EventOperator &op, Trace &trace, size_t index) {
    return std::make_shared<EventOperator>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitResolvingOperator(ResolvingOperator &op, Trace &trace,
                                                                        size_t index) {
    return std::make_shared<ResolvingOperator>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitDerivingOperator(DerivingOperator &op, Trace &trace,
                                                                       size_t index) {
    return std::make_shared<DerivingOperator>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitPredicate(Predicate &op, Trace &trace, size_t index) {
    return std::make_shared<Predicate>(op);
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitUniversalQuantifier(UniversalQuantifier &op, Trace &trace,
                                                                          size_t index) {
    // A x in D. X(x) <-> X(A x in D. x)
    if (auto rewExpr = swapQuantifierAndTempUnary(std::make_shared<UniversalQuantifier>(op), trace, index))
        return rewExpr;

    visitExpression(op, trace, index);

    // A x in D. A y in D. x & y <-> A x, y in D. x & y
    if (auto inner = std::dynamic_pointer_cast<UniversalQuantifier>(op.getLeft())) {
        std::vector<Variable> variables;
        for (const auto &var : op.getVariables())
            variables.push_back(var);
        std::shared_ptr<Expression> innerLeft = inner->getLeft();
        auto newUniversal = std::make_shared<UniversalQuantifier>(innerLeft, variables);
        for (const auto &var : inner->getVariables()) {
            newUniversal->removeVariable(var.getName());
            newUniversal->addVariable(var.getName(), var.getDomain());
        }
        return newUniversal;
    }

    // A x, y in D. x <-> A x in D. x or A x in D. E x in D. x <-> A x in D. x
    auto newExpr = removeUnusedVariables(std::make_shared<UniversalQuantifier>(op));

    return newExpr;
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitExistentialQuantifier(ExistentialQuantifier &op, Trace &trace,
                                                                            size_t index) {
    // E x in D. X(x) <-> X(E x in D. x)
    if (auto rewExpr = swapQuantifierAndTempUnary(std::make_shared<ExistentialQuantifier>(op), trace, index))
        return rewExpr;

    visitExpression(op, trace, index);

    // E x in D. E y in D. x & y <-> E x, y in D. x & y
    if (auto inner = std::dynamic_pointer_cast<ExistentialQuantifier>(op.getLeft())) {
        std::vector<Variable> variables;
        for (const auto &var : op.getVariables())
            variables.push_back(var);
        for (const auto &var : inner->getVariables())
            variables.push_back(var);
        std::shared_ptr<Expression> innerLeft = inner->getLeft();
        auto newExistential = std::make_shared<ExistentialQuantifier>(innerLeft, variables);
        for (const auto &var : inner->getVariables()) {
            newExistential->removeVariable(var.getName());
            newExistential->addVariable(var.getName(), var.getDomain());
        }
        return newExistential;
    }

    // E x, y in D. x <-> E x in D. x or E x in D. A x in D. x <-> A x in D. x
    auto newExpr = removeUnusedVariables(std::make_shared<ExistentialQuantifier>(op));

    return newExpr;
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitQuoteQuantifier(QuoteQuantifier &op, Trace &trace, size_t index) {
    // Q[0., 0.5] x in D. X(x) <-> X(Q[0., 0.5] x in D. x)
    if (auto rewExpr = swapQuantifierAndTempUnary(std::make_shared<QuoteQuantifier>(op), trace, index))
        return rewExpr;

    visitExpression(op, trace, index);

    // Q[0., 0.5] x in D. Q[0., 0.5] y in D. x & y <-> Q[0., 0.5] x, y in D. x & y
    if (auto inner = std::dynamic_pointer_cast<QuoteQuantifier>(op.getLeft())) {
        auto opProb{op.getQuoteInterval()};
        auto innerProb{inner->getQuoteInterval()};
        if (opProb.getMin() == innerProb.getMin() and opProb.getMax() == innerProb.getMax()) {
            std::vector<Variable> variables;
            for (const auto &var : op.getVariables())
                variables.push_back(var);
            for (const auto &var : inner->getVariables())
                variables.push_back(var);
            std::shared_ptr<Expression> innerLeft = inner->getLeft();
            auto newQuote = std::make_shared<QuoteQuantifier>(innerLeft, variables, opProb);
            for (const auto &var : inner->getVariables()) {
                newQuote->removeVariable(var.getName());
                newQuote->addVariable(var.getName(), var.getDomain());
            }
            return newQuote;
        }
    }

    // Q[0., 0.5] x, y in D. x <-> Q[0., 0.5] x in D. x or C[0., 0.5] x in D. A x in D. x <-> A x in D. x
    auto newExpr = removeUnusedVariables(std::make_shared<QuoteQuantifier>(op));

    return newExpr;
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitCountingQuantifier(CountingQuantifier &op, Trace &trace,
                                                                         size_t index) {
    // C=2 x in D. X(x) <-> X(C=2 x in D. x)
    if (auto rewExpr = swapQuantifierAndTempUnary(std::make_shared<CountingQuantifier>(op), trace, index))
        return rewExpr;

    visitExpression(op, trace, index);

    // C=2 x in D. C=2 y in D. x & y <-> C=2 x, y in D. x & y
    if (auto inner = std::dynamic_pointer_cast<CountingQuantifier>(op.getLeft())) {
        auto opOpera{op.getOperator()};
        auto innerOpera{inner->getOperator()};
        auto opNum{op.getNum()};
        auto innerNum{inner->getNum()};
        if (opOpera == innerOpera and opNum == innerNum) {
            std::vector<Variable> variables;
            for (const auto &var : op.getVariables())
                variables.push_back(var);
            for (const auto &var : inner->getVariables())
                variables.push_back(var);
            std::shared_ptr<Expression> innerLeft = inner->getLeft();
            auto newCounting = std::make_shared<CountingQuantifier>(innerLeft, variables, opOpera, opNum);
            for (const auto &var : inner->getVariables()) {
                newCounting->removeVariable(var.getName());
                newCounting->addVariable(var.getName(), var.getDomain());
            }
            return newCounting;
        }
    }

    // C=2 x, y in D. x <-> C=2 x in D. x or C=2 x in D. A x in D. x <-> A x in D. x
    auto newExpr = removeUnusedVariables(std::make_shared<CountingQuantifier>(op));

    return newExpr;
}

std::shared_ptr<Expression> HydraRewriteVisitor::visitEqualityOperator(EqualityOperator &op, Trace &trace,
                                                                       size_t index) {
    return std::make_shared<EqualityOperator>(op);
}

std::shared_ptr<Expression>
HydraRewriteVisitor::swapQuantifierAndTempUnary(const std::shared_ptr<FirstOrderOperator> &oper, Trace &trace,
                                                size_t index) {
    if (auto firstOrder = std::dynamic_pointer_cast<Unary>(oper)) {
        if (!std::dynamic_pointer_cast<FirstOrderOperator>(firstOrder->getLeft()))
            if (auto tempUnary = std::dynamic_pointer_cast<Unary>(firstOrder->getLeft())) {
                firstOrder->setLeft(tempUnary->getLeft());
                tempUnary->setLeft(firstOrder);
                return tempUnary->accept(*this, trace, index);
            }
    }
    return nullptr;
}

std::shared_ptr<Expression>
HydraRewriteVisitor::removeUnusedVariables(const std::shared_ptr<FirstOrderOperator> &oper) {
    if (auto unary = std::dynamic_pointer_cast<Unary>(oper)) {
        for (const auto &var : std::vector<Variable>{oper->getVariables()}) {
            std::string varName = var.getName();
            if (not hasUsedQuantifiedVariable(*unary->getLeft(), varName))
                oper->removeVariable(varName);
        }
        if (oper->getVariables().empty())
            return unary->getLeft();
    }
    return std::dynamic_pointer_cast<Expression>(oper);
}

bool HydraRewriteVisitor::hasUsedQuantifiedVariable(Expression &expr, std::string &varName) {
    bool result = false;
    if (auto *atomic = dynamic_cast<Atomic *>(&expr)) {
        return atomic->to_string() == varName;
    } else if (auto *predicate = dynamic_cast<Predicate *>(&expr)) {
        if (varName == predicate->getName())
            return true;
        return std::any_of(
            predicate->getArguments().begin(), predicate->getArguments().end(),
            [varName](const std::pair<std::string, Predicate::Argument> &arg) { return varName == arg.first; });
    } else if (auto *unary = dynamic_cast<Unary *>(&expr)) {
        if (auto *firstOrder = dynamic_cast<FirstOrderOperator *>(unary))
            for (const auto &var : firstOrder->getVariables())
                if (varName == var.getName())
                    return false;
        result = hasUsedQuantifiedVariable(*unary->getLeft(), varName);
    } else if (auto *binary = dynamic_cast<Binary *>(&expr)) {
        result = hasUsedQuantifiedVariable(*binary->getLeft(), varName) ||
                 hasUsedQuantifiedVariable(*binary->getRight(), varName);
    } else if (auto *nary = dynamic_cast<NaryInterface *>(&expr)) {
        for (const auto &childExpr : *nary)
            result = result || hasUsedQuantifiedVariable(*childExpr, varName);
    }
    return result;
}
