#include "mtl_event_consuming.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "mtl_event_consuming_visitor.h"
#include <iostream>

MTLEventConsuming::MTLEventConsuming(const Formula &formula) : MonitorInterface((formula)) {}

std::vector<Verdict> MTLEventConsuming::evaluate(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return {Verdict{0, false}};
    }
    // Copy formula before working on it
    auto formulaCopy = formula.deepCopy();
    MtlRewriteVisitor rw;
    MTLEventConsumingVisitor ev;
    // Rewrite
    rw.rewrite(formulaCopy);
    for (size_t i = 0; i < trace.getLength(); ++i) {
        ev.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }
    return {Verdict{0, formulaCopy.to_string() == "true"}};
}
std::string MTLEventConsuming::to_string() const { return "TODO"; }
