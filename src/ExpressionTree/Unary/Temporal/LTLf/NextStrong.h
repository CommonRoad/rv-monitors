#pragma once


#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing the strong next.
 */
class NextStrong : public Unary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextStrong(const std::shared_ptr<Expression> &left) : Unary(
            nextStrongSymbol, left), interval(std::make_shared<Interval>()) {}

    explicit NextStrong(const std::shared_ptr<Expression> &left, std::shared_ptr<Interval> interval) : Unary(
            nextStrongSymbol + interval->to_string(), left), interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<NextStrong>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override {
        return visitor.visitNextStrong(*this, event);
    }
};



