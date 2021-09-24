#pragma once

#include <monitors/expression_tree/binary/binary.h>

/**
 * Class representing an Implication.
 */
class Implication : public Binary {
  public:
    /**
     * Constructor.
     * @param left Left expression of the implication.
     * @param right Right expression of the implication.
     */
    Implication(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right)
        : Binary(implicationSymbol, left, right) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;
};
