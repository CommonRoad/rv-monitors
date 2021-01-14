#pragma once


#include "ExpressionTree/Binary/Binary.h"
#include "ExpressionTree/Interval.h"

/**
 * Class representing a Release.
 */
class Release : public Binary {
private:
    std::shared_ptr<Interval> interval;
public:

    /**
     * Constructor.
     * @param left Left expression of the release.
     * @param right Right expression of the release.
     */
    Release(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right
    ) : Binary(releaseSymbol, left, right), interval(std::make_shared<Interval>()) {}

    explicit Release(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right,
                    std::shared_ptr<Interval> interval) :
                    Binary(releaseSymbol + interval->to_string(), left, right),
                    interval(std::move(interval)) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Event &event) override;

};



