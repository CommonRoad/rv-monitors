#pragma once


#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the weak next.
 */
class NextWeak : public Unary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextWeak(const std::shared_ptr<Expression> &left) : Unary(
            nextWeakSymbol, left), TemporalOperator(std::make_shared<Interval>())  {}

    explicit NextWeak(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            futureSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<NextWeak>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override {
        return visitor.visitNextWeak(*this, trace, index);
    }
};



