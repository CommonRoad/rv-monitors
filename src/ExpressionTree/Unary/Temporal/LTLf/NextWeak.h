#pragma once


#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing the weak next.
 */
class NextWeak : public Unary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextWeak(const std::shared_ptr<Expression> &left) : Unary(
            nextWeakSymbol, left), interval(std::make_shared<Interval>())  {}

    explicit NextWeak(const std::shared_ptr<Expression> &left, std::shared_ptr<Interval> interval) : Unary(
            futureSymbol + interval->to_string(), left), interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<NextWeak>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitNextWeak(*this, event);
    }
};



