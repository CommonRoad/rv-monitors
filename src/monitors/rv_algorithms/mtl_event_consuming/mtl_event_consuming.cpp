#include "mtl_event_consuming.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "mtl_event_consuming_visitor.h"
#include <iostream>

MTLEventConsuming::MTLEventConsuming(const Formula &formula) : MonitorInterface((formula)) {}

std::map<std::string, std::vector<Verdict>> MTLEventConsuming::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        result[formulas.at(0).getId()] = {Verdict{false}};
        return result;
    }
    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
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
    result[formulas.at(0).getId()] = {Verdict{formulaCopy.to_string() == "true"}};
    return result;
}
std::string MTLEventConsuming::to_string() const { return "TODO"; }
