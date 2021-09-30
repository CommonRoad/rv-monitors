#pragma once

#include "monitors/expression_tree/nary/nary.h"

/**
 * Class representing an And.
 */
class And : public Nary<std::set<std::shared_ptr<Expression>, NaryInterface::SharedPtrExprCompare>, And> {
  public:
    /**
     * Constructor.
     * @param expressions Set of expressions connected with and.
     */
    explicit And(const decltype(expressions) &expressions) : Nary(andSymbol, expressions) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId,
                                                       std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                                       size_t parentId) override;
};
