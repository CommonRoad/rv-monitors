#pragma once

#include "monitors/expression_tree/expression.h"

/**
 * Class representing an atomic proposition.
 */
class Atomic : public Expression {
  public:
    /**
     * Constructor.
     * @param symbol Symbol of the Atomic.
     */
    explicit Atomic(std::string symbol) : Expression(std::move(symbol)) {}

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId, std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline, size_t parentId) override;
};
