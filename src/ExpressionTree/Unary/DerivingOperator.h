//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_DERIVINGOPERATOR_H
#define RV_MONITORS_DERIVINGOPERATOR_H

#include "Unary.h"
#include "Trace/Event.h"

/**
 * Class for the DerivingOperator that applies an event to a formula.
 */
class DerivingOperator : public Unary {
private:
    Event event;

public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit DerivingOperator(const std::shared_ptr<Expression> &left, Event event) : Unary(
            event.to_string(), left), event(std::move(event)) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &traceVis) override;

    [[nodiscard]] std::string to_string() const override;
};


#endif //RV_MONITORS_DERIVINGOPERATOR_H
