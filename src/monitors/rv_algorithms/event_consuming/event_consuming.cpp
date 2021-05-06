#include "event_consuming.h"
#include "event_consuming_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/rewriting/rewrite_visitor.h"
#include <iostream>
#include <memory>

Formula EventConsuming::eventConsumingFuture(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    rewrite_visitor rw;
    event_consuming_visitor ec;

    // Rewrite
    rw.rewrite(formulaCopy);
    for (int i = 0; i < trace.getLength(); ++i) {
        ec.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            //            std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            // std::cout << "Formula became true in event " << i << std::endl;
            break;
        }
    }
    return formulaCopy;
}

Formula EventConsuming::eventConsumingPast(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    rewrite_visitor rw;
    event_consuming_visitor ec;

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
            // std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            // std::cout << "Formula became true in event " << i << std::endl;
            break;
        }
    }
    return formulaCopy;
}

bool EventConsuming::eventConsuming(const Formula &formula, Trace &trace, bool past) {
    if (!past) {
        return eventConsumingFuture(formula, trace).to_string() == "true";
    }
    return eventConsumingPast(formula, trace).to_string() == "true";
}