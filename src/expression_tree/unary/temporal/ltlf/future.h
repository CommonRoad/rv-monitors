#pragma once

#include <utility>

#include "expression_tree/temporal_operator.h"
#include "expression_tree/unary/unary.h"

/**
 * Class representing the future operator.
 */
class Future : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Future(const std::shared_ptr<Expression> &left)
        : Unary(futureSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Future(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(futureSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<Future>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override {
        return visitor.visitFuture(*this, trace, index);
    }
};
