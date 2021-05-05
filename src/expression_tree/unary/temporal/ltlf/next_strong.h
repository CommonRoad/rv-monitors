#pragma once

#include "expression_tree/temporal_operator.h"
#include "expression_tree/unary/unary.h"

/**
 * Class representing the strong next.
 */
class NextStrong : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextStrong(const std::shared_ptr<Expression> &left)
        : Unary(nextStrongSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit NextStrong(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(nextStrongSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<NextStrong>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override {
        return visitor.visitNextStrong(*this, trace, index);
    }
};
