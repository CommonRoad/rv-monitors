#pragma once


#include <ExpressionTree/Binary/Binary.h>
#include "ExpressionTree/Atomic/Interval.h"

/**
 * Class representing an Until.
 */
class Until : public Binary {
private:
    std::shared_ptr<Interval> interval;
public:
    /**
     * Constructor.
     * @param left Left expression of the until.
     * @param right Right expression of the until.
     */
    Until(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right) :
    Binary(untilSymbol, left, right), interval(std::make_shared<Interval>()) {}

    explicit Until(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right,
                     std::shared_ptr<Interval> interval) :
            Binary(untilSymbol + interval->to_string(), left, right),
            interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override;
};



