//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_RESOLVING_OPERATOR_H
#define RV_MONITORS_RESOLVING_OPERATOR_H

#include <utility>

#include "monitors/trace/trace.h"
#include "unary.h"

/**
 * Class for the ResolvingOperator that applies an event to a formula.
 */
class ResolvingOperator : public Unary {
  private:
    Trace trace;
    size_t index;

  public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit ResolvingOperator(const std::shared_ptr<Expression> &left, Trace trace, size_t idx)
        : Unary(trace.getEvent(idx).to_string(), left), trace(std::move(trace)), index(idx) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t *maxId) override;

    [[nodiscard]] std::string to_string() const override;
};

#endif // RV_MONITORS_RESOLVING_OPERATOR_H
