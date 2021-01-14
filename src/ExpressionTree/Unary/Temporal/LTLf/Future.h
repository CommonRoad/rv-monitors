#pragma once

#include <utility>

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the future operator.
 */
class Future : public Unary, public TemporalOperator{
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Future(const std::shared_ptr<Expression> &left) : Unary(
            futureSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Future(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            futureSymbol + interval->to_string(), left), TemporalOperator(std::make_shared<Interval>()) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Future>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitFuture(*this, event);
    }
};



