#include <iostream>
#include <memory>
#include "Rewriting/RewriteVisitor.h"
#include "EventConsuming.h"
#include "EventConsumingVisitor.h"
#include "ExpressionTree/Atomic/Constant.h"
#include "ExpressionTree/Unary/EventOperator.h"

Formula EventConsuming::eventConsumingFuture(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return Formula(std::make_shared<Constant>(false));
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    RewriteVisitor rw;
    EventConsumingVisitor ec;

    // Rewrite
    rw.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    for (int i = 0; i < trace.getLength(); ++i) {
        std::vector<Event> newEventVector(&trace.getTrace()[0], &trace.getTrace()[i + 1]);
        Trace newTrace = Trace(newEventVector);
        ec.consumeEvent(formulaCopy, newTrace);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
//            std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            //std::cout << "Formula became true in event " << i << std::endl;
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
    RewriteVisitor rw;
    EventConsumingVisitor ec;

    // Update terminal event
    trace.setTerminalEvent(false);
    // Rewrite
    rw.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    // Traverse trace backwards
    for (size_t i = traceRef.size() - 1; i >= 0 && i < traceRef.size(); --i) {
        std::vector<Event> newEventVector(&trace.getTrace()[i], &trace.getTrace()[trace.getLength()]);
        std::reverse(newEventVector.begin(), newEventVector.end());
        Trace newTrace = Trace(newEventVector);
        ec.consumeEvent(formulaCopy, newTrace);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            //std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            //std::cout << "Formula became true in event " << i << std::endl;
            break;
        }

    }
    return formulaCopy;
}

bool EventConsuming::eventConsuming(const Formula &formula, Trace &trace, bool past){
    if(!past){
        return eventConsumingFuture(formula, trace).to_string() == "true";
    }
    return eventConsumingPast(formula, trace).to_string() == "true";

}