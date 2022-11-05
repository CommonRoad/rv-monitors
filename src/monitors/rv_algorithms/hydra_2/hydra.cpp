#include "hydra.h"
#include "../../rewriting/hydra_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include <iostream>
#include <spdlog/spdlog.h>

std::map<std::string, std::vector<Verdict>> Hydra2::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;

    resetMonitors();
    setTraceMonitors(trace);
    for (const auto &mon : monitors)
        result[mon->getFormulaId()].reserve(trace.getLength());

    for (size_t i{0}; i < trace.getLength(); ++i)
        for (const auto &mon : monitors)
            result[mon->getFormulaId()].emplace_back(mon->step(0, i));

    return result;
}

std::string Hydra2::to_string() const {
    std::string str;
    for (const auto &mon : monitors)
        str += "G(" + mon->to_string() + ")";
    return str;
}

void Hydra2::resetMonitors() {
    for (const auto &mon : monitors)
        mon->reset();
}

void Hydra2::setTraceMonitors(const Trace &tra) {
    for (const auto &mon : monitors)
        mon->setTrace(tra);
}

void Hydra2::createMonitor(const std::vector<Formula> &formulas, DomainCollection &domainCollection,
                           FunctionCollection &functionCollection, bool connectedEval, bool reordering,
                           bool shortCircuiting, const std::map<std::string, std::array<double, 2>> &predicateCosts) {
    HydraCreationVisitor2 hcv{
        HydraCreationVisitor2(predicateCosts, domainCollection, functionCollection, reordering, shortCircuiting)};

    for (const auto &form : formulas) {
        if (form.getId().empty())
            throw std::logic_error("Formula has no valid unique ID.");
        auto formulaCopy{form.deepCopy()};
        if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
            formulaCopy = Formula(globally->getLeft());
        HydraRewriteVisitor rwv;
        rwv.rewrite(formulaCopy);
        formulaCopy.setId(form.getId());

        monitors.push_back(hcv.createMonitor(formulaCopy, 0, connectedEval));
    }
}

Hydra2::Hydra2(const std::vector<Formula> &formulas, DomainCollection domainCollection,
               FunctionCollection functionCollection, bool connectedEval, bool reordering, bool shortCircuiting,
               const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : MonitorInterface(formulas, AlgorithmType::hydra_2_mtl) {
    createMonitor(formulas, domainCollection, functionCollection, connectedEval, reordering, shortCircuiting,
                  predicateCosts);
}
