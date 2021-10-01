#pragma once

#include "monitors/expression_tree/nary/nary.h"
#include "monitors/expression_tree/nary/nary_interface.h"

/**
 * Class representing an Xor.
 */
class Xor : public Nary<std::multiset<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>, Xor> {
  public:
    /**
     * Constructor.
     * @param expressions Multiset of expressions connected with xor.
     */
    explicit Xor(const decltype(expressions) &expressions) : Nary(xorSymbol, expressions) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId) override;
};
