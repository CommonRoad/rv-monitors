#pragma once

#include <utility>

#include "monitors/trace/trace.h"
#include "unary.h"

/**
 * Class for the ResolvingOperator that applies an event to a formula.
 */
class ResolvingOperator : public Unary {
  private:
    const std::shared_ptr<Trace> trace;
    size_t index;

  public:
    /**
     * Constructor.
     * @param left Expression event should be applied to.
     * @param event Event.
     */
    explicit ResolvingOperator(const std::shared_ptr<Expression> &left, const std::shared_ptr<Trace> &trace, size_t idx)
        : Unary(trace->getEvent(idx).to_string(), left), trace(trace), index(idx) {}

    /**
     * Getter for event.
     * @return The operator's event.
     */
    [[nodiscard]] Event getEvent();

    [[nodiscard]] std::shared_ptr<Expression> clone() override;

    [[nodiscard]] std::shared_ptr<Expression> accept(ExpressionVisitor &visitor, const std::shared_ptr<Trace> &trace,
                                                     size_t index) override;

    [[nodiscard]] std::shared_ptr<Hydra2Monitor> accept(HydraCreationVisitor2 &visitor, size_t parentId,
                                                        bool connectedEval) override;

    [[nodiscard]] std::shared_ptr<HydraMonitor> accept(HydraCreationVisitor &visitor) override;

    [[nodiscard]] std::string to_string() const override;
};
