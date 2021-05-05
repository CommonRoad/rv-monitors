#pragma once

#include "expression_tree/temporal_operator.h"
#include "expression_tree/unary/unary.h"

/**
 * Class representing the Historically operator.
 */
class Historically : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Historically(const std::shared_ptr<Expression> &left)
        : Unary(onceSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Historically(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(historicallySymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<Historically>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override {
        return visitor.visitHistorically(*this, trace, index);
    }
};
