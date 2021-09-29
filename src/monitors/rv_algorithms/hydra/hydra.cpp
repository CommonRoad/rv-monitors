#include "hydra.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include <iostream>

std::vector<Verdict> Hydra::evaluate(Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        return {Verdict{0, false}};
    }

    std::vector<Verdict> result;
    result.reserve(trace.getLength());

    monitor->reset();
    monitor->setTrace(trace);

    for (size_t i{0}; i < trace.getLength(); ++i)
        result.emplace_back(monitor->step());

    return result;
}
std::string Hydra::to_string() const { return "TODO"; }

void Hydra::createMonitor(const Formula &formula) {
    // Copy formula before working on it
    auto formulaCopy{formula.deepCopy()};
    if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
        formulaCopy = Formula(globally->getLeft());

    MtlRewriteVisitor rw;
    rw.rewrite(formulaCopy);

    //   formula = formulaCopy;

    HydraCreationVisitor hc;
    monitor = hc.createMonitor(formulaCopy);
}

Hydra::Hydra(const Formula &formula) : MonitorInterface(formula) { createMonitor(formula); }
