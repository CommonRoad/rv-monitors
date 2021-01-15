#include <iostream>
#include <memory>
#include "MTLRewriteVisitor.h"
#include "MTLEventConsuming.h"
#include "MTLResolvingPastVisitor.h"

bool MTLEventConsuming::eventConsuming(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return false;
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    MTLRewriteVisitor rw;
    MTLResolvingPastVisitor ec;

    // Rewrite
    rw.rewrite(formulaCopy);
    auto &traceRef = trace.getTraceRef();
    for (int i = 0; i < trace.getLength(); ++i) {
        auto first = trace.getTrace().begin();
        auto last = trace.getTrace().begin() + i;
        std::vector<Event> newVec(first, last);
        Trace newTrace = Trace(newVec);
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
    return formulaCopy.to_string() == "true";
}
