#pragma once

#include <memory>

class Expression;

class Unary;

class Binary;

class nary_interface;

class Atomic;

class Constant;

class Constant;

class And;

class Or;

class Xor;

class Implication;

class Not;

class Future;

class Globally;

class NextStrong;

class NextWeak;

class Once;

class Previously;

class Release;

class Until;

class Since;

class Historically;

class Trace;

class EventOperator;

class ResolvingOperator;

class DerivingOperator;

class Equivalence;

class Predicate;

class UniversalQuantifier;

class ExistentialQuantifier;

class QuoteQuantifier;

class CountingQuantifier;

/**
 * Parent class to apply the visitor pattern to the expression tree.
 * The functionality of the single visit functions depends on the inheriting class.
 */
class ExpressionVisitor {
  public:
    void visitExpression(Expression &expr, Trace &trace, size_t index);

    /**
     * Visits an Atomic.
     * @param expr Atomic.
     * @param trace Trace.
     * @return Transformed Atomic as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitAtomic(Atomic &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Constant.
     * @param expr Constant.
     * @param trace Trace.
     * @return Transformed Constant as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitConstant(Constant &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an And.
     * @param expr And.
     * @param trace Trace.
     * @return Transformed And as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitAnd(And &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Or.
     * @param expr Or.
     * @param trace Trace.
     * @return Transformed Or as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitOr(Or &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Xor.
     * @param expr Xor.
     * @param trace Trace.
     * @return Transformed Xor as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitXor(Xor &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Implication.
     * @param expr Implication.
     * @param trace Trace.
     * @return Transformed Implication as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitImplication(Implication &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Equivalence.
     * @param expr Equivalence.
     * @param trace Trace.
     * @return Transformed Equivalence as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitEquivalence(Equivalence &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Not.
     * @param expr Not.
     * @param trace Trace.
     * @return Transformed Not as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitNot(Not &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Future.
     * @param expr Future.
     * @param trace Trace.
     * @return Transformed Future as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitFuture(Future &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Globally.
     * @param expr Globally.
     * @param trace Trace.
     * @return Transformed Globally as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitGlobally(Globally &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an NextStrong.
     * @param expr NextStrong.
     * @param trace Trace.
     * @return Transformed NextStrong as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitNextStrong(NextStrong &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an NextWeak.
     * @param expr NextWeak.
     * @param trace Trace.
     * @return Transformed NextWeak as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitNextWeak(NextWeak &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Once.
     * @param expr Once.
     * @param trace Trace.
     * @return Transformed Once as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitOnce(Once &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Previously.
     * @param expr Previously.
     * @param trace Trace.
     * @return Transformed Previously as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitPreviously(Previously &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Release.
     * @param expr Release.
     * @param trace Trace.
     * @return Transformed Release as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitRelease(Release &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an Until.
     * @param expr Until.
     * @param trace Trace.
     * @return Transformed Until as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitUntil(Until &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a Since.
     * @param expr Since.
     * @param trace Trace.
     * @return Transformed Since as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitSince(Since &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a Historically.
     * @param expr Historically.
     * @param trace Trace.
     * @return Transformed Historically as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitHistorically(Historically &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an EventOperator.
     * @param expr EventOperator.
     * @param trace Trace.
     * @return Transformed EventOperator as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitEventOperator(EventOperator &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a ResolvingOperator.
     * @param expr ResolvingOperator.
     * @param trace Trace.
     * @return Transformed EventOperator as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitResolvingOperator(ResolvingOperator &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a DerivingOperator.
     * @param expr DerivingOperator.
     * @param trace Trace.
     * @return Transformed EventOperator as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitDerivingOperator(DerivingOperator &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a Predicate.
     * @param expr Predicate.
     * @param trace Trace.
     * @return Transformed EventOperator as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitPredicate(Predicate &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits an UniversalQuantifier.
     * @param expr Expression.
     * @param trace Trace.
     * @return Transformed UniversalQuantifier as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitUniversalQuantifier(UniversalQuantifier &expr, Trace &trace,
                                                                 size_t index) = 0;

    /**
     * Visits an ExistentialQuantifier.
     * @param expr Expression.
     * @param trace Trace.
     * @return Transformed ExistentialQuantifier as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitExistentialQuantifier(ExistentialQuantifier &expr, Trace &trace,
                                                                   size_t index) = 0;

    /**
     * Visits a QuoteQuantifier.
     * @param expr Expression.
     * @param trace Trace.
     * @return Transformed QuoteQuantifier as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitQuoteQuantifier(QuoteQuantifier &expr, Trace &trace, size_t index) = 0;

    /**
     * Visits a CountingQuantifier.
     * @param expr Expression.
     * @param trace Trace.
     * @return Transformed CountingQuantifier as a shared_ptr.
     */
    virtual std::shared_ptr<Expression> visitCountingQuantifier(CountingQuantifier &expr, Trace &trace,
                                                                size_t index) = 0;
};
