#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

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

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override {
        return visitor.visitHistorically(*this, trace, index);
    }

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId,
                                                       std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                                       size_t parentId) override {
        return nullptr;
    }
};
