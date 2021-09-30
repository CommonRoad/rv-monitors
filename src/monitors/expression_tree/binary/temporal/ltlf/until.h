#pragma once

#include "monitors/expression_tree/temporal_operator.h"
#include <monitors/expression_tree/binary/binary.h>

/**
 * Class representing an Until.
 */
class Until : public Binary, public TemporalOperator {
  public:
    /**
     * Constructor.
     * @param left Left expression of the until.
     * @param right Right expression of the until.
     */
    Until(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right)
        : Binary(untilSymbol, left, right), TemporalOperator(std::make_shared<Interval>()) {}

    explicit Until(const std::shared_ptr<Expression> &left, const std::shared_ptr<Expression> &right,
                   const std::shared_ptr<Interval> &interval)
        : Binary(untilSymbol + interval->to_string(), left, right), TemporalOperator(interval) {}

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId,
                                                       std::map<std::string, std::shared_ptr<HydraMonitor>> *baseline,
                                                       size_t parentId) override;
};
