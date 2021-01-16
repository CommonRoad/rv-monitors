#pragma once

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the previously operator.
 */
class Previously : public Unary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Previously(const std::shared_ptr<Expression> &left) : Unary(
            previousSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Previously(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            previousSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Previously>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override {
        return visitor.visitPreviously(*this, trace, index);
    }
};



