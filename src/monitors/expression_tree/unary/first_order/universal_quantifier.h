#ifndef RVMONITORS_UNIVERSAL_QUANTIFIER_H
#define RVMONITORS_UNIVERSAL_QUANTIFIER_H

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"

class UniversalQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     */
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by quantifier.
     */
    explicit UniversalQuantifier(std::shared_ptr<Expression> &left, Variable &variable);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the universal quantifier expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;

};

#endif // RVMONITORS_UNIVERSAL_QUANTIFIER_H
