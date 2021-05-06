#pragma once

#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing a boolean constant, i.e. true or false.
 */
class Constant : public Expression {
  private:
    bool value;

  public:
    /**
     * Constructor.
     * @param value Truth value of the constant.
     */
    explicit Constant(bool value) : Expression(value ? "true" : "false"), value(value) {}

    /**
     * Getter for value.
     * @return value.
     */
    [[nodiscard]] bool getValue() const;

    [[nodiscard]] std::string to_string() const override;

    std::shared_ptr<Expression> clone() override;

    std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override;
};