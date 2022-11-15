#include "hydra.h"
#include "../../rewriting/hydra_rewrite_visitor.h"
#include "hydra_creation_visitor.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include <iostream>
#include <spdlog/spdlog.h>

std::map<std::string, std::vector<Verdict>> Hydra2::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        spdlog::info("Hydra2::evaluate | Trace is empty");
        for (const auto &mon : monitors)
            result[mon->getFormulaId()] = {};
        return result;
    }

    resetMonitors();
    setTraceMonitors(trace);

    if (!earlyTermination)
        for (const auto &mon : monitors)
            result[mon->getFormulaId()].reserve(trace.getLength());
    else
        for (const auto &mon : monitors)
            result[mon->getFormulaId()].push_back(true); // only single verdict required

    if (!earlyTermination)
        for (size_t i{0}; i < trace.getLength(); ++i)
            for (const auto &mon : monitors)
                result[mon->getFormulaId()].emplace_back(mon->step(0, i));
    else {
        std::vector<std::string> terminatedMonitors;
        for (size_t i{0}; i < trace.getLength(); ++i) {
            for (const auto &mon : monitors)
                if (!std::any_of(terminatedMonitors.begin(), terminatedMonitors.end(),
                                 [mon](const std::string &monId) { return mon->getFormulaId() == monId; })) {
                    auto res{mon->step(0, i)};
                    result[mon->getFormulaId()].at(0) = result[mon->getFormulaId()].at(0).value && res.value;
                    if (!result[mon->getFormulaId()].at(0).value)
                        terminatedMonitors.push_back(mon->getFormulaId());
                }
            if (monitors.size() == terminatedMonitors.size())
                break;
        }
    }
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

void Hydra2::createMonitor(const std::vector<Formula> &formulas, bool connectedEval, bool reordering,
                           bool shortCircuiting, const std::map<std::string, std::array<double, 2>> &predicateCosts) {
    HydraCreationVisitor2 hcv{HydraCreationVisitor2(predicateCosts, reordering, shortCircuiting)};
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

Hydra2::Hydra2(const std::vector<Formula> &formulas, bool connectedEval, bool reordering, bool shortCircuiting,
               bool earlyTermination, const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : MonitorInterface(formulas, AlgorithmType::hydra_2_mtl, true, earlyTermination) {
    createMonitor(formulas, connectedEval, reordering, shortCircuiting, predicateCosts);
}
