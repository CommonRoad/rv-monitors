#pragma once

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
        : Unary(trace.getBooleanEvent(idx).to_string(), left), trace(std::move(trace)), index(idx) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event<bool> getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, Trace &trace, size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    [[nodiscard]] std::string to_string() const override;
};
