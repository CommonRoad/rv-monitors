#ifndef RVMONITORS_COUNTING_QUANTIFIER_H
#define RVMONITORS_COUNTING_QUANTIFIER_H

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"

class ProbabilityInterval {
  public:
    /**
     * Constructor.
     *
     * @param min Minimum probability.
     * @param max Maximum probability.
     */
    explicit ProbabilityInterval(float min, float max);

    /**
     * Returns minimum of probability.
     *
     * @return Minimum.
     */
    [[nodiscard]] float getMin() const;

    /**
     * Returns maximum of probability.
     *
     * @return Maximum.
     */
    [[nodiscard]] float getMax() const;

  private:
    float min; //**< Minimum probability. */
    float max; //**< Maximum probability. */

};

class CountingQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     * @param probInterval Probability interval.
     */
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables, ProbabilityInterval &probInterval);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by expression.
     * @param probInterval Probability interval.
     */
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, Variable &variable, ProbabilityInterval &probInterval);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the counting quantifier expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;

    /**
     * Returns probability interval.
     *
     * @return Probability interval.
     */
    const ProbabilityInterval &getProbInterval() const;

  private:
    ProbabilityInterval probInterval; //**< Probability interval. */

};

#endif // RVMONITORS_COUNTING_QUANTIFIER_H
