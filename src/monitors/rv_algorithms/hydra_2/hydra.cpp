#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include <iostream>
#include <spdlog/spdlog.h>

std::map<std::string, std::vector<Verdict>> Hydra2::evaluate(const std::shared_ptr<Trace> &trace,
                                                             const std::vector<std::shared_ptr<DomainStorage>> &domains,
                                                             const std::vector<std::string> &formulaIDs) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if ((trace == nullptr or trace->getTrace().empty()) and traceLength == 0) {
        std::cout << "Hydra2::evaluate | Trace is empty\n";
        for (const auto &mon : monitors)
            result[mon->getFormulaId()] = {};
        return result;
    }

    if (trace != nullptr and !trace->getTrace().empty()) {
        resetMonitors();
        setTraceMonitors(trace, domains);
    }

    if (!earlyTermination)
        evalCompleteTrace(result, formulaIDs);
    else
        evalEarlyTermination(result, formulaIDs);

    return result;
}

void Hydra2::evalEarlyTermination(std::map<std::string, std::vector<Verdict>> &result,
                                  const std::vector<std::string> &formulaIDs) {
    std::vector<std::shared_ptr<Hydra2Monitor>> relMonitors;
    for (const auto &mon : monitors)
        if (result.find(mon->getFormulaId()) == result.end() and
            (formulaIDs.empty() or std::any_of(formulaIDs.begin(), formulaIDs.end(),
                                               [mon](const std::string &fID) { return fID == mon->getFormulaId(); }))) {
            result[mon->getFormulaId()].emplace_back(
                true); // only single verdict required and use "invalid" time step to indicate early termination
            relMonitors.push_back(mon);
        }

    for (size_t i{0}; i < traceLength; ++i) {
        for (const auto &mon : relMonitors) {
            result[mon->getFormulaId()].at(0) = result[mon->getFormulaId()].at(0).value and mon->step(0, i).value;
            if (!result[mon->getFormulaId()].at(0).value)
                return;
        }
    }
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

void Hydra2::setTraceMonitors(const std::shared_ptr<Trace> &tra,
                              const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    for (const auto &mon : monitors)
        mon->setTrace(tra, domains);
    traceLength = tra->getTrace().size();
}

void Hydra2::createMonitor(const std::vector<Formula> &formulas, bool connectedEval, bool reordering,
                           bool shortCircuiting) {
    HydraCreationVisitor2 hcv{HydraCreationVisitor2(predicateCosts, reordering, shortCircuiting,
                                                    std::vector<std::shared_ptr<DomainStorage>>(domains))};
    for (const auto &form : formulas) {
        if (form.getId().empty())
            throw std::logic_error("Formula has no valid unique ID.");
        auto formulaCopy{form.deepCopy()};
        if (auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr()))
            formulaCopy = Formula(globally->getLeft());
        HydraRewriteVisitor rwv;
        rwv.rewrite(formulaCopy);
        formulaCopy.setId(form.getId());

        // baseline monitors are kept for each creation run -> connected eval
        monitors.push_back(hcv.createMonitor(formulaCopy, 0, connectedEval));
    }
}

Hydra2::Hydra2(const std::vector<Formula> &formulas, bool connectedEval, bool reordering, bool shortCircuiting,
               bool earlyTermination, const std::vector<std::shared_ptr<DomainStorage>> &domains,
               const std::map<std::string, std::array<double, 2>> &predicateCosts)
    : MonitorInterface(formulas, AlgorithmType::hydra_2_mtl, true, earlyTermination, domains),
      predicateCosts(predicateCosts) {
    createMonitor(formulas, connectedEval, reordering, shortCircuiting);
}

std::map<std::string, std::array<double, 2>> &Hydra2::getPredicateCost() { return predicateCosts; }

void Hydra2::evalCompleteTrace(std::map<std::string, std::vector<Verdict>> &result,
                               const std::vector<std::string> &formulaIDs) {
    std::vector<std::shared_ptr<Hydra2Monitor>> relMonitors;
    for (const auto &mon : monitors)
        if (result.find(mon->getFormulaId()) == result.end() and
            (formulaIDs.empty() or std::any_of(formulaIDs.begin(), formulaIDs.end(),
                                               [mon](const std::string &fID) { return fID == mon->getFormulaId(); }))) {
            result[mon->getFormulaId()].reserve(traceLength);
            relMonitors.push_back(mon);
        }

    for (size_t i{0}; i < traceLength; ++i)
        for (const auto &mon : relMonitors) {
            spdlog::debug("Monitor evaluation: " + std::to_string(i) + " - " + mon->getFormulaId());
            mon->addDomainValues({});
            result[mon->getFormulaId()].emplace_back(mon->step(0, i));
        }
}

std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>>
Hydra2::getViolatingDomainValues() const {
    std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>> violations;
    for (const auto &mon : this->monitors)
        violations[mon->getFormulaId()] = mon->getViolatingDomainValues();
    return violations;
}

TreeVis Hydra2::to_tree() {
    std::vector<size_t> roots;
    std::map<size_t, std::tuple<std::string, std::vector<size_t>>> treeComplete;
    for (const auto &mon : monitors) {
        roots.push_back(mon->getMonitorID());
        mon->to_tree(treeComplete);
    }
    std::map<size_t, std::string> nodeTypes;
    std::map<size_t, std::vector<size_t>> tree;
    for (const auto &[key, value] : treeComplete) {
        nodeTypes.insert({key, std::get<0>(value)});
        tree.insert({key, std::get<1>(value)});
    }
    return {nodeTypes, tree, roots};
}

const std::map<size_t, std::string> &TreeVis::getNodeTypes() const { return nodeTypes; }

const std::map<size_t, std::vector<size_t>> &TreeVis::getTree() const { return tree; }

const std::vector<size_t> &TreeVis::getRootIDs() const { return rootIDs; }

TreeVis::TreeVis(const std::map<size_t, std::string> &nodeTypes, const std::map<size_t, std::vector<size_t>> &tree,
                 const std::vector<size_t> &rootIDs)
    : nodeTypes(nodeTypes), tree(tree), rootIDs(rootIDs) {}
