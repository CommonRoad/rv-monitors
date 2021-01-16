#include <iostream>
#include <memory>
#include "MTLRewriteVisitor.h"
#include "MTLEventConsuming.h"
#include "MTLResolvingPastVisitor.h"
#include "MTLDerivingFutureVisitor.h"

bool MTLEventConsuming::eventConsuming(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return false;
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    MTLRewriteVisitor rw;
    MTLResolvingPastVisitor rp;
    MTLDerivingFutureVisitor df;

    // Rewrite
    rw.rewrite(formulaCopy);
    std::cout << formulaCopy.to_string() << '\n';
    auto &traceRef = trace.getTraceRef();
    for (int i = 0; i < trace.getLength(); ++i) {
        std::vector<Event> newEventVector(&trace.getTrace()[0], &trace.getTrace()[i + 1]);
        Trace newTrace = Trace(newEventVector);
        rp.consumeEvent(formulaCopy, newTrace);
        std::cout << formulaCopy.to_string() << '\n';
        rw.rewrite(formulaCopy);
        std::cout << formulaCopy.to_string() << '\n';
        if (formulaCopy.to_string() == "false") {
//            std::cout << "Formula violated in event " << i << std::endl;
            break;
        }
        if (formulaCopy.to_string() == "true") {
            //std::cout << "Formula became true in event " << i << std::endl;
            break;
        }
        df.consumeEvent(formulaCopy, newTrace);
        std::cout << formulaCopy.to_string() << '\n';
        rw.rewrite(formulaCopy);
        std::cout << formulaCopy.to_string() << '\n';
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
