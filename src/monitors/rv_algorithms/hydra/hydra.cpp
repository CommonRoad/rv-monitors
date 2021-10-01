#include "hydra.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/ltlf/globally.h"
#include <iostream>

std::map<std::string, std::vector<Verdict>> Hydra::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        for (const auto &mon : monitors)
            result[mon->getFormula().getId()] = {Verdict{0, false}};
        return result;
    }

    resetMonitors();
    setTraceMonitors(trace);
    for (const auto &mon : monitors)
        result[mon->getFormula().getId()].reserve(trace.getLength());

    for (size_t i{0}; i < trace.getLength(); ++i)
        for (const auto &mon : monitors)
            result[mon->getFormula().getId()].emplace_back(mon->step(0));

    return result;
}
std::string Hydra::to_string() const { return "TODO"; }

void Hydra::resetMonitors() {
    for (const auto &mon : monitors)
        mon->reset();
}

void Hydra::setTraceMonitors(const Trace &tr) {
    for (const auto &mon : monitors)
        mon->setTrace(tr);
}

void Hydra::createMonitor(const std::vector<Formula> &formulas) {
    for (const auto &form : formulas) {
        auto formulaCopy{form.deepCopy()};
        if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
            formulaCopy = Formula(globally->getLeft());
        MtlRewriteVisitor rw;
        rw.rewrite(formulaCopy);
        HydraCreationVisitor hc;
        monitors.push_back(hc.createMonitor(formulaCopy, 0));
    }
}

Hydra::Hydra(const std::vector<Formula> &formulas) : MonitorInterface(formulas) { createMonitor(formulas); }
