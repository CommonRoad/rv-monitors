#pragma once

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"

/**
 * Class representing the existential quantifier.
 */
class ExistentialQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     */
    explicit ExistentialQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by quantifier.
     */
    explicit ExistentialQuantifier(std::shared_ptr<Expression> &left, Variable &variable);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the existential quantifier expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId, bool connectedEval) override;

};
