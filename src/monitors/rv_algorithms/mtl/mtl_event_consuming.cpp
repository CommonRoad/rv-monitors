#include "mtl_event_consuming.h"
#include "mtl_event_consuming_visitor.h"
#include "mtl_rewrite_visitor.h"
#include <iostream>
#include <memory>

bool MTLEventConsuming::eventConsuming(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return false;
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    mtl_rewrite_visitor rw;
    MTLEventConsumingVisitor ev;
    // Rewrite
    rw.rewrite(formulaCopy);
    for (int i = 0; i < trace.getLength(); ++i) {
        ev.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }
    return formulaCopy.to_string() == "true";
}
