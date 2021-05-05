#pragma once

#include "expression_tree/temporal_operator.h"
#include "expression_tree/unary/unary.h"

/**
 * Class representing the previously operator.
 */
class Previously : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Previously(const std::shared_ptr<Expression> &left)
        : Unary(previousSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Previously(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(previousSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<Previously>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override {
        return visitor.visitPreviously(*this, trace, index);
    }
};
