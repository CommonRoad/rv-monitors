#ifndef RVMONITORS_COUNTING_QUANTIFIER_H
#define RVMONITORS_COUNTING_QUANTIFIER_H

#include "monitors/expression_tree/unary/unary.h"
#include "monitors/expression_tree/first_order_operator.h"

class ProbabilityInterval {

  private:
    /// Minimum of probability
    float min;

    /// Maximum of probability
    float max;

  public:
    /// Constructor
    explicit ProbabilityInterval(float min, float max);

    /**
     * Returns minimum of probability.
     *
     * @return Minimum
     */
    [[nodiscard]] float getMin() const;

    /**
     * Returns maximum of probability.
     *
     * @return Maximum
     */
    [[nodiscard]] float getMax() const;

};

class CountingQuantifier : public Unary, public FirstOrderOperator {

  private:
    /// Probability interval
    ProbabilityInterval probInterval;

  public:
    /// First constructor
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables, ProbabilityInterval &probInterval);

    /// Second constructor
    explicit CountingQuantifier(std::shared_ptr<Expression> &left, Variable &variable, ProbabilityInterval &probInterval);

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;

    /**
     * Returns probability interval.
     *
     * @return Probability interval
     */
    const ProbabilityInterval &getProbInterval() const;

};

#endif // RVMONITORS_COUNTING_QUANTIFIER_H
