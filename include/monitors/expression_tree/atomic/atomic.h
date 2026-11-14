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

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
