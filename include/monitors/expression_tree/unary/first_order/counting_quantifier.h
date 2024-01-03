#pragma once

#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing counting quantifier.
 */
class CountingQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * Operator type of counting quantifier.
     */
    enum Operator { LESS_EQ, EQ, GREATER_EQ };

    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     * @param oper Operator type.
     * @param num Number.
     */
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables, Operator oper,
                                size_t num);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by quantifier.
     * @param oper Operator type.
     * @param num Number.
     */
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, Variable &variable, Operator oper, size_t num);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the counting quantifier expression.
     *
     * @return Clones expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    /**
     * Returns operator type.
     *
     * @return Operator type.
     */
    const Operator &getOperator() const;

    /**
     * Returns number.
     *
     * @return Number.
     */
    size_t getNum() const;

  private:
    Operator op; //**< Operator type. */
    size_t num;  //**< Number. */
};