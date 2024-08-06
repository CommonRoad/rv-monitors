#pragma once

#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/expression_tree/unary/unary.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"

/**
 * Class representing the universal quantifier.
 */
class UniversalQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     */
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by quantifier.
     */
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, Variable &variable);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the universal quantifier expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
