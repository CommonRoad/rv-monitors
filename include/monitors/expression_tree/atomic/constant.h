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

    std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                       size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;
};
