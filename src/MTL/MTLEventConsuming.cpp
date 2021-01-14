#include <iostream>
#include <memory>
#include "MTLRewriteVisitor.h"
#include "MTLEventConsuming.h"
#include "MTLEventConsumingVisitor.h"

bool MTLEventConsuming::eventConsuming(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return false;
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    MTLRewriteVisitor rw;
    MTLEventConsumingVisitor ec;

    // Rewrite
    rw.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    for (auto & i : traceRef) {
        ec.consumeEvent(formulaCopy, i);
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
    return formulaCopy.to_string() == "true";
}
