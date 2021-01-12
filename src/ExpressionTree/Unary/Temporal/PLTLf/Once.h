#pragma once

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing the Once operator.
 */
class Once : public Unary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Once(const std::shared_ptr<Expression> &left) : Unary(
            onceSymbol, left), interval(std::make_shared<Interval>()) {}

    explicit Once(const std::shared_ptr<Expression> &left, std::shared_ptr<Interval> interval) : Unary(
            onceSymbol + interval->to_string(), left), interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Once>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitOnce(*this, event);
    }
};



