#pragma once

#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/expression_tree/unary/unary.h"

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

    /**
     * Accept function for visitor pattern for existential quantifier.
     *
     * @param visitor Visitor.
     * @param trace Event.
     * @param index Current (max) index.
     *
     * @return Expression of visit existential quantifier.
     */
    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    /**
     * Accept function for visitor pattern for Hydra2 monitor.
     *
     * @param visitor Visitor.
     * @param parentId ID of parent monitor.
     * @param connectedEval Boolean indicating whether connected evaluation should be performed.
     * @return Hydra2 existential quantifier monitor.
     */
    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    /**
     * Accept function for visitor pattern for original Hydra monitor.
     *
     * @param visitor Visitor.
     * @return Hydra existential quantifier monitor.
     */
    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    /**
     * Extracts domain variables from expression.
     *
     * @return List of domain variables.
     */
    [[nodiscard]] std::vector<Variable> extractVariables() const override;
};
