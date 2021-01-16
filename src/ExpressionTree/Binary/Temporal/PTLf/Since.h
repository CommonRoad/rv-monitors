//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_SINCE_H
#define RV_MONITORS_SINCE_H

#include <ExpressionTree/Binary/Binary.h>
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing a Since.
 */
class Since : public Binary, public TemporalOperator {
public:
    /**
     * Constructor.
     * @param left Left expression of the until.
     * @param right Right expression of the until.
     */
    Since(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right) :
            Binary(sinceSymbol, left, right), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Since(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right,
                   const std::shared_ptr<Interval>& interval) :
            Binary(sinceSymbol + interval->to_string(), left, right),
            TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override;
};


#endif //RV_MONITORS_SINCE_H
