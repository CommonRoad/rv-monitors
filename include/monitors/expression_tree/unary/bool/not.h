#pragma once

#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing a Not.
 */
class Not : public Unary {
  public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Not(const std::shared_ptr<Expression> &left) : Unary(notSymbol, left) {}

    std::shared_ptr<Expression> clone() override;

    std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
