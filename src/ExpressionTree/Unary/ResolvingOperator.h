//
// Created by Sebastian Maierhofer on 14.01.21.
//

#ifndef RV_MONITORS_RESOLVINGOPERATOR_H
#define RV_MONITORS_RESOLVINGOPERATOR_H

#include "Unary.h"
#include "Trace/Trace.h"

/**
 * Class for the ResolvingOperator that applies an event to a formula.
 */
class ResolvingOperator : public Unary{
private:
    Trace trace;
public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit ResolvingOperator(const std::shared_ptr<Expression> &left, Trace trace) : Unary(
            trace.getLastEvent().to_string(), left), trace(std::move(trace)) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &tr) override;

    [[nodiscard]] std::string to_string() const override;
};


#endif //RV_MONITORS_RESOLVINGOPERATOR_H
