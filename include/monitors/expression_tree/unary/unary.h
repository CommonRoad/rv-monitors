#pragma once

#include "monitors/expression_tree/expression.h"
#include "monitors/expression_tree/first_order_operator.h"
/**
 * Parent class for all unary expressions.
 */
class Unary : public Expression {
  protected:
    std::shared_ptr<Expression> left;

  public:
    /**
     * Constructor.
     * @param symbol Symbol of the respective expression.
     * @param left Left expression.
     */
    explicit Unary(std::string symbol, std::shared_ptr<Expression> left)
        : Expression(std::move(symbol)), left(std::move(left)) {}

    /**
     * Getter for left expression.
     * @return Left expression.
     */
    [[nodiscard]] const std::shared_ptr<Expression> &getLeft() const;

    /**
     * Setter for left expression.
     * @param left New left expression.
     */
    void setLeft(const std::shared_ptr<Expression> &leftExpr);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Extracts domain variables from expression.
     *
     * @return List of domain variables.
     */
    [[nodiscard]] std::vector<Variable> extractVariables() const override;
};
