#include "hydra.h"
#include "../../rewriting/hydra_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include <iostream>
#include <spdlog/spdlog.h>

std::map<std::string, std::vector<Verdict>> Hydra::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        spdlog::info("Hydra::evaluate | Trace is empty");
        for (const auto &mon : monitors)
            result[mon->getFormulaId()] = {};
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
        str += mon->to_string();
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

void Hydra::createMonitor(const std::vector<Formula> &formulas, bool connectedEval, bool reordering,
                          bool shortCircuiting, const std::map<std::string, std::array<double, 2>> &predicateCosts) {
    for (const auto &pred : predicateCosts)
        assert(pred.second[0] < 0 or pred.second[0] > 1 or pred.second[1] < 0 or pred.second[1] > 1);
    HydraCreationVisitor hcv{HydraCreationVisitor(predicateCosts, reordering, shortCircuiting)};
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

Hydra::Hydra(const std::vector<Formula> &formulas, bool connectedEval, bool reordering, bool shortCircuiting,
             const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : MonitorInterface(formulas) {
    for (const auto &pred : predicateCosts)
        assert(pred.second[0] < 0 or pred.second[0] > 1 or pred.second[1] < 0 or pred.second[1] > 1);
    createMonitor(formulas, connectedEval, reordering, shortCircuiting, predicateCosts);
}
