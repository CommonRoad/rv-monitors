#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing the Globally operator.
 */
class Globally : public Unary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Globally(const std::shared_ptr<Expression> &left)
        : Unary(globallySymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Globally(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval> &interval)
        : Unary(globallySymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override { return std::make_shared<Globally>(*this); }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override {
        return visitor.visitGlobally(*this, trace, index);
    }

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId, std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline, size_t parentId) override {
        return nullptr;
    }
};
