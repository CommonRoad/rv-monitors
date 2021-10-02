#pragma once

#include "monitors/expression_tree/nary/nary.h"

/**
 * Class representing an Or.
 */
class Or : public Nary<std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>, Or> {
  public:
    /**
     * Constructor.
     * @param expressions Set of expressions connected with or.
     */
    explicit Or(const decltype(expressions) &expressions) : Nary(orSymbol, expressions) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;
};
