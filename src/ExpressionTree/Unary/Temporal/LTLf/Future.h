#pragma once

#include <utility>

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing the future operator.
 */
class Future : public Unary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Future(const std::shared_ptr<Expression> &left) : Unary(
            futureSymbol, left), interval(std::make_shared<Interval>()) {}

    explicit Future(const std::shared_ptr<Expression> &left, std::shared_ptr<Interval> interval) : Unary(
            futureSymbol + interval->to_string(), left), interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Future>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitFuture(*this, event);
    }
};



