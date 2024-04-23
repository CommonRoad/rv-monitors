#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

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

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
