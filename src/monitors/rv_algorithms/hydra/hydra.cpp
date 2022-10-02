#include "hydra.h"
#include "../../rewriting/hydra_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include <iostream>

std::map<std::string, std::vector<Verdict>> Hydra::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getBooleanTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        result[formulas.at(0).getId()] = {};
        return result;
    }

    monitor->reset();
    monitor->setTrace(trace);

    for (size_t i{0}; i < trace.getLength(); ++i)
        result[monitor->getFormulaId()].emplace_back(monitor->concreteStep());

    return result;
}

std::string Hydra::to_string() const { return "G(" + monitor->to_string() + ")"; }

void Hydra::createMonitor(const Formula &formula) {
    if (formula.getId().empty())
        throw std::logic_error("Formula has no valid unique ID.");
    // Copy formula before working on it
    auto formulaCopy{formula.deepCopy()};
    if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
        formulaCopy = Formula(globally->getLeft());

    HydraRewriteVisitor rwv;
    rwv.rewrite(formulaCopy);
    formulaCopy.setId(formula.getId());

    HydraCreationVisitor hcv;
    monitor = hcv.createMonitor(formulaCopy);
}

Hydra::Hydra(const std::vector<Formula> &formulas) : MonitorInterface(formulas, AlgorithmType::hydra_mtl) {
    createMonitor(formulas.at(0));
}
