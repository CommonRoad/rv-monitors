#pragma once

#include "expression_tree/temporal_operator.h"
#include "expression_tree/unary/unary.h"

/**
 * Class representing the weak next.
 */
class NextWeak : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit NextWeak(const std::shared_ptr<Expression> &left)
        : Unary(nextWeakSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit NextWeak(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(futureSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<NextWeak>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override {
        return visitor.visitNextWeak(*this, trace, index);
    }
};
