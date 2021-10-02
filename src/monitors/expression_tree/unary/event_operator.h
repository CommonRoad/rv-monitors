#pragma once

#include "monitors/trace/trace.h"
#include "unary.h"

/**
 * Class for the EventOperator that applies an event to a formula.
 */
class EventOperator : public Unary {
  private:
    Trace trace;
    size_t index;

  public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit EventOperator(const std::shared_ptr<Expression> &left, Trace trace, size_t idx)
        : Unary(trace.getEvent(idx).to_string(), left), trace(std::move(trace)), index(idx) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::string to_string() const override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor, size_t parentId, bool connectedEval) override;
};
