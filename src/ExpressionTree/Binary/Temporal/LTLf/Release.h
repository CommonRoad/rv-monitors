#pragma once


#include "ExpressionTree/Binary/Binary.h"
#include "ExpressionTree/TemporalOperator.h"

/**
 * Class representing a Release.
 */
class Release : public Binary, public TemporalOperator {
public:

    /**
     * Constructor.
     * @param left Left expression of the release.
     * @param right Right expression of the release.
     */
    Release(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right
    ) : Binary(releaseSymbol, left, right), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Release(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right,
                    const std::shared_ptr<Interval>& interval) :
                    Binary(releaseSymbol + interval->to_string(), left, right),
                    TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override;

};



