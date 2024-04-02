#include "monitors/rv_algorithms/ltl_event_consuming//event_consuming.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/rewriting/rewrite_visitor.h"
#include "monitors/rv_algorithms/ltl_event_consuming/event_consuming_visitor.h"
#include <iostream>
#include <memory>

EventConsumingLTL::EventConsumingLTL(const std::vector<Formula> &formulas, bool pastEvaluation,
                                     bool formulaCompatibilityCheck)
    : MonitorInterface(formulas, AlgorithmType::event_consuming_ltl, formulaCompatibilityCheck),
      pastEvaluation(pastEvaluation) {}

Formula EventConsumingLTL::eventConsumingFuture(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
    RewriteVisitor rwv;
    EventConsumingVisitor ecv;

    // Rewrite
    rwv.rewrite(formulaCopy);
    for (size_t i = 0; i < trace.getLength(); ++i) {
        ecv.consumeEvent(formulaCopy, trace, i);
        rwv.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }
    return formulaCopy;
}

Formula EventConsumingLTL::eventConsumingPast(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
    RewriteVisitor rwv;
    EventConsumingVisitor ecv;

    // Update terminal event
    trace.setTerminalEvent(false);
    // Rewrite
    rwv.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    // Traverse trace backwards
    for (int i = static_cast<int>(traceRef.size()) - 1; i >= 0; --i) {
        ecv.consumeEvent(formulaCopy, trace, static_cast<size_t>(i));
        rwv.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }
    return formulaCopy;
}

std::map<std::string, std::vector<Verdict>>
EventConsumingLTL::evaluate(Trace &trace, const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    std::map<std::string, std::vector<Verdict>> result;
    if (!pastEvaluation)
        result[formulas.at(0).getId()] = {Verdict{eventConsumingFuture(trace).to_string() == "true"}};
    else
        result[formulas.at(0).getId()] = {Verdict{eventConsumingPast(trace).to_string() == "true"}};
    return result;
}
std::string EventConsumingLTL::to_string() const { return "TODO"; }
