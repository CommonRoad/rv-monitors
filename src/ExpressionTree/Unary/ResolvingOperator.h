//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_RESOLVINGOPERATOR_H
#define RV_MONITORS_RESOLVINGOPERATOR_H

#include <utility>

#include "Trace/Trace.h"
#include "Unary.h"

/**
 * Class for the ResolvingOperator that applies an event to a formula.
 */
class ResolvingOperator : public Unary {
  private:
    Trace trace;
    int index;

  public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit ResolvingOperator(const std::shared_ptr<Expression> &left, Trace trace, int idx)
        : Unary(trace.getEvent(idx).to_string(), left), trace(std::move(trace)), index(idx) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, int index) override;

    [[nodiscard]] std::string to_string() const override;
};

#endif // RV_MONITORS_RESOLVINGOPERATOR_H
