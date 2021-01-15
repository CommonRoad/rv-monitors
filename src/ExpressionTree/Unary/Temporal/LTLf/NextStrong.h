#pragma once


#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the strong next.
 */
class NextStrong : public Unary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextStrong(const std::shared_ptr<Expression> &left) : Unary(
            nextStrongSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit NextStrong(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            nextStrongSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<NextStrong>(*this);
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace) override {
        return visitor.visitNextStrong(*this, trace);
    }
};



