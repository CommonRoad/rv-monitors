#ifndef RVMONITORS_EXISTENCE_QUANTIFIER_H
#define RVMONITORS_EXISTENCE_QUANTIFIER_H

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"

class ExistentialQuantifier : public Unary, public FirstOrderOperator {

  public:
    /// First constructor
    explicit ExistentialQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables);

    /// Second constructor
    explicit ExistentialQuantifier(std::shared_ptr<Expression> &left, Variable &variable);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;

};

#endif // RVMONITORS_EXISTENCE_QUANTIFIER_H
