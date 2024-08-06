#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing the Once operator.
 */
class Once : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Once(const std::shared_ptr<Expression> &left)
        : Unary(onceSymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Once(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(onceSymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
