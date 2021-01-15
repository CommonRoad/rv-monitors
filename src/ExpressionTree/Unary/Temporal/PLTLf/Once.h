#pragma once

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the Once operator.
 */
class Once : public Unary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Once(const std::shared_ptr<Expression> &left) : Unary(
            onceSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Once(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            onceSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Once>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace) override {
        return visitor.visitOnce(*this, trace);
    }
};



