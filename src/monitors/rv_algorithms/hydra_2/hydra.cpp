#include "hydra.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include <iostream>
#include <utility>

std::map<std::string, std::vector<Verdict>> Hydra::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        for (const auto &mon : monitors)
            result[mon->getFormulaId()] = {Verdict{0, false}};
        return result;
    }

    resetMonitors();
    setTraceMonitors(trace);
    for (const auto &mon : monitors)
        result[mon->getFormulaId()].reserve(trace.getLength());

    for (size_t i{0}; i < trace.getLength(); ++i)
        for (const auto &mon : monitors)
            result[mon->getFormulaId()].emplace_back(mon->step(0, i));

    return result;
}

std::string Hydra::to_string() const {
    std::string str;
    for (const auto &mon : monitors)
        str += mon->to_string() + '\n';
    return str;
}

void Hydra::resetMonitors() {
    for (const auto &mon : monitors)
        mon->reset();
}

void Hydra::setTraceMonitors(const Trace &tra) {
    for (const auto &mon : monitors)
        mon->setTrace(tra);
}

void Hydra::createMonitor(const std::vector<Formula> &formulas, bool connectedEval,
                          std::vector<std::tuple<std::string, double, double>> predicateCosts) {
    HydraCreationVisitor hcv{HydraCreationVisitor(std::move(predicateCosts))};
    for (const auto &form : formulas) {
        if (form.getId().empty())
            throw std::logic_error("Formula has no valid unique ID.");
        auto formulaCopy{form.deepCopy()};
        if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
            formulaCopy = Formula(globally->getLeft());
        MtlRewriteVisitor rwv;
        rwv.rewrite(formulaCopy);
        formulaCopy.setId(form.getId());

        monitors.push_back(hcv.createMonitor(formulaCopy, 0, connectedEval));
    }
}

Hydra::Hydra(const std::vector<Formula> &formulas, bool connectedEval,
             std::vector<std::tuple<std::string, double, double>> predicateCosts)
    : MonitorInterface(formulas) {
    createMonitor(formulas, connectedEval, std::move(predicateCosts));
}
