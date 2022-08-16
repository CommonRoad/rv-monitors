#ifndef RVMONITORS_UNIVERSAL_QUANTIFIER_H
#define RVMONITORS_UNIVERSAL_QUANTIFIER_H

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"

class UniversalQuantifier : public Unary, public FirstOrderOperator {

  public:
    /// First constructor
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, std::vector<std::shared_ptr<Variable>> &variables);

    /// Second constructor
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, std::shared_ptr<Variable> &variable);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;

};

#endif // RVMONITORS_UNIVERSAL_QUANTIFIER_H
