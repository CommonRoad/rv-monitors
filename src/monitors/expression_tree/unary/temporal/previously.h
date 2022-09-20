#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/rv_algorithms/hydra_2/hydra_monitors/hydra_monitor.h"

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

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override {
        return visitor.visitPreviously(*this, trace, index);
    }

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                       bool connectedEval) override {
        return visitor.visitPreviously(*this, parentId, connectedEval);
    }
};
