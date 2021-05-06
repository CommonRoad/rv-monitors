#pragma once

#include "monitors/expression_tree/binary/binary.h"

/**
 * Class representing an Equivalence.
 */
class Equivalence : public Binary {

  public:
    /**
     * Constructor.
     * @param left Left expression of the equivalence.
     * @param right Right expression of the equivalence.
     */
    Equivalence(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right)
        : Binary(equivalenceSymbol, left, right) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(expression_visitor &visitor, Trace &trace, int index) override;
};
