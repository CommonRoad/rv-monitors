#include "event_consuming.h"
#include "event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/rewriting/rewrite_visitor.h"
#include <iostream>
#include <memory>
#include <utility>

EventConsumingLTL::EventConsumingLTL(const std::vector<Formula> &formulas, bool pastEvaluation)
    : MonitorInterface((std::move(formulas))), pastEvaluation(pastEvaluation) {}

Formula EventConsumingLTL::eventConsumingFuture(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
    RewriteVisitor rw;
    EventConsumingVisitor ec;

    // Rewrite
    rw.rewrite(formulaCopy);
    for (size_t i = 0; i < trace.getLength(); ++i) {
        ec.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
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
    RewriteVisitor rw;
    EventConsumingVisitor ec;

    // Update terminal event
    trace.setTerminalEvent(false);
    // Rewrite
    rw.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    // Traverse trace backwards
    for (size_t i = traceRef.size() - 1; i >= 0 && i < traceRef.size(); --i) {
        ec.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }
    return formulaCopy;
}

std::map<std::string, std::vector<Verdict>> EventConsumingLTL::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    if (!pastEvaluation)
        result[formulas.at(0).getId()] = {Verdict{eventConsumingFuture(trace).to_string() == "true"}};
    else
        result[formulas.at(0).getId()] = {Verdict{eventConsumingPast(trace).to_string() == "true"}};
    return result;
}
std::string EventConsumingLTL::to_string() const { return "TODO"; }
