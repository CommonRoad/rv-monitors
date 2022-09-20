#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

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

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override {
        return visitor.visitNextStrong(*this, trace, index);
    }

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                       bool connectedEval) override {
        return visitor.visitNext(*this, parentId, connectedEval);
    }
};
