#pragma once


#include <utility>

#include "ExpressionTree/Unary/Unary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing the Globally operator.
 */
class Globally : public Unary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Expression.
     */
    explicit Globally(const std::shared_ptr<Expression> &left) : Unary(
            globallySymbol, left), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Globally(const std::shared_ptr<Expression> &left, const std::shared_ptr<Interval>& interval) : Unary(
            globallySymbol + interval->to_string(), left), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override {
        return std::make_shared<Globally>(this->getLeft()->clone(), this->getInterval()->clone());
    }

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override {
        return visitor.visitGlobally(*this, trace, index);
    }
};



