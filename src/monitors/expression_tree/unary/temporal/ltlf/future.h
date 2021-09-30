#pragma once

#include <utility>

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

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

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override {
        return visitor.visitFuture(*this, trace, index);
    }

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId,
                                                       std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                                       size_t parentId) override {
        return nullptr;
    }
};
