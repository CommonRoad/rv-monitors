#pragma once

#include "monitors/expression_tree/first_order_operator.h"
#include "monitors/expression_tree/unary/unary.h"

/**
 * Class representing a quote interval used by quote quantifier.
 */
class QuoteInterval {
  public:
    /**
     * Constructor.
     *
     * @param min Minimum probability.
     * @param max Maximum probability.
     */
    explicit QuoteInterval(float min, float max);

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

/**
 * Class representing a quote quantifier.
 */
class QuoteQuantifier : public Unary, public FirstOrderOperator {
  public:
    /**
     * First constructor.
     *
     * @param left Child expression.
     * @param variables Variables used by quantifier.
     * @param quoteInterval Quote interval.
     */
    explicit QuoteQuantifier(std::shared_ptr<Expression> &left, std::vector<Variable> &variables,
                             QuoteInterval &quoteInterval);

    /**
     * Second constructor.
     *
     * @param left Child expression.
     * @param variable Variable used by expression.
     * @param quoteInterval Quote interval.
     */
    explicit QuoteQuantifier(std::shared_ptr<Expression> &left, Variable &variable, QuoteInterval &quoteInterval);

    /**
     * Creates string representation of expression.
     *
     * @return String representing expression.
     */
    [[nodiscard]] std::string to_string() const override;

    /**
     * Clones the quote quantifier expression.
     *
     * @return Cloned expression.
     */
    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    /**
     * Returns quote interval.
     *
     * @return Quote interval.
     */
    const QuoteInterval &getQuoteInterval() const;

  private:
    QuoteInterval quoteInterval; //**< Quote interval. */
};
