#pragma once


#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing the Globally operator.
 */
class Globally : public Unary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Globally(const std::shared_ptr<Expression> &left) : Unary(
            globallySymbol, left), interval(std::make_shared<Interval>()) {}

    explicit Globally(const std::shared_ptr<Expression> &left, std::shared_ptr<Interval> interval) : Unary(
            globallySymbol + interval->to_string(), left), interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Globally>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitGlobally(*this, event);
    }
};



