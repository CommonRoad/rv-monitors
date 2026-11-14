#include "monitors/rv_algorithms/hydra_2/hydra.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/rewriting/hydra_rewrite_visitor.h"
#include "monitors/rv_algorithms/hydra_2/hydra_creation_visitor.h"
#include <algorithm>
#include <spdlog/spdlog.h>

std::map<std::string, std::vector<Verdict>> Hydra2::evaluate(const std::shared_ptr<Trace> &trace,
                                                             const std::vector<std::shared_ptr<DomainStorage>> &domains,
                                                             const std::vector<std::string> &formulaIDs) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if ((trace == nullptr or trace->getTrace().empty()) and traceLength == 0) {
        spdlog::debug("Hydra2::evaluate | Trace is empty\n");
        for (const auto &formulaId : formulaIds)
            result[formulaId] = {};
        return result;
    }

    // executed when new trace should be used (pass nullptr trace if trace should be reused)
    if (trace != nullptr and !trace->getTrace().empty()) {
        resetMonitors();
        setTraceMonitors(trace, domains);
    }

    const bool shouldStore = (formulaEvaluationMode == globally);
    for (const auto &mon : monitors)
        mon->setStoreVariableResults(shouldStore);

    if (formulaEvaluationMode == globally) {
        evalCompleteTrace(result, formulaIDs);
    } else {
        evalEarlyTermination(result, formulaIDs);
    }

    return result;
}

void Hydra2::evalEarlyTermination(std::map<std::string, std::vector<Verdict>> &result,
                                  const std::vector<std::string> &formulaIDs) const {
    std::vector<std::shared_ptr<Hydra2Monitor>> relMonitors;
    std::vector<std::string> relFormulaIds;
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        if (const auto &formulaId{formulaIds.at(idx)};
            result.find(formulaId) == result.end() and
            (formulaIDs.empty() or std::find(formulaIDs.begin(), formulaIDs.end(), formulaId) != formulaIDs.end())) {
            result[formulaId].emplace_back(0,
                                           true); // initially, we had only single verdict and uses "invalid" time step
                                                  // to indicate early termination; however, for better interpretation
                                                  // of result, we provide time stamp where specification is violated
            relMonitors.push_back(monitors.at(idx));
            relFormulaIds.push_back(formulaId);
        }

    // Cache the verdict slots so the per-step loop avoids a string-keyed map lookup. The monitor
    // evaluation order is left unchanged on purpose: under early termination it determines which
    // rule is reported when several are violated at the same earliest time step.
    std::vector<std::vector<Verdict> *> verdictRefs;
    verdictRefs.reserve(relMonitors.size());
    for (const auto &formulaId : relFormulaIds)
        verdictRefs.push_back(&result[formulaId]);

    // In initial_step mode, only the first time step is evaluated, but for all formulas
    const size_t numSteps{formulaEvaluationMode == initial_step ? std::min<size_t>(traceLength, 1) : traceLength};
    for (size_t i{0}; i < numSteps; ++i) {
        for (size_t m{0}; m < relMonitors.size(); ++m) {
            const auto &mon = relMonitors[m];
            spdlog::debug("Monitor evaluation: " + std::to_string(i) + " - " + relFormulaIds[m]);
            mon->addDomainValues({}); // triggers adding of domain values, e.g., to predicates
            verdictRefs[m]->at(0) = mon->step(0, i);
            // In globally_early_termination mode, we terminate as soon as the formula is not satisfied at any time step
            if (formulaEvaluationMode == globally_early_termination && !verdictRefs[m]->at(0).value) {
                return;
            }
        }
        pruneVerdicts(relMonitors, i);
    }
}

std::string Hydra2::to_string() const {
    std::string str;
    for (const auto &mon : monitors)
        str += "G(" + mon->to_string() + ")";
    return str;
}

void Hydra2::resetMonitors() const {
    for (const auto &mon : monitors)
        mon->reset();
}

void Hydra2::setTraceMonitors(const std::shared_ptr<Trace> &tra,
                              const std::vector<std::shared_ptr<DomainStorage>> &domains) {
    for (const auto &mon : monitors)
        mon->setTrace(tra, domains);
    traceLength = tra->getTrace().size();
    timeStamps.clear();
    for (const auto &event : tra->getTrace())
        timeStamps.push_back(event.getTimeStep());
}

void Hydra2::createMonitor(const std::vector<Formula> &formulas, const bool connectedEval, const bool reordering,
                           const bool shortCircuiting) {
    auto hcv{HydraCreationVisitor2(predicateCosts, reordering, shortCircuiting, std::vector(domains))};
    for (const auto &form : formulas) {
        if (form.getId().empty())
            throw std::logic_error("Formula has no valid unique ID.");
        auto formulaCopy{form.deepCopy()};
        // If we are running in initial_step mode, we do not want to strip any operators,
        // since we evaluate the formula as-is
        if (formulaEvaluationMode != initial_step) {
            if (const auto globally = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr())) {
                formulaCopy = Formula(globally->getLeft());
            }
        }
        HydraRewriteVisitor rwv;
        rwv.rewrite(formulaCopy);
        formulaCopy.setId(form.getId());

        // baseline monitors are kept for each creation run -> connected eval; with connected evaluation, formulas with
        // identical (sub)structure share the same monitor, so the formula IDs are stored separately
        monitors.push_back(hcv.createMonitor(formulaCopy, 0, connectedEval));
        formulaIds.push_back(form.getId());
    }
    // after all monitors are created since the number of parents of shared monitors is known only then
    for (const auto &mon : monitors)
        mon->configureVerdictStorage();
}

Hydra2::Hydra2(const std::vector<Formula> &formulas, const bool connectedEval, const bool reordering,
               const bool shortCircuiting, const FormulaEvaluationMode formulaEvaluationMode,
               const std::vector<std::shared_ptr<DomainStorage>> &domains,
               const std::map<std::string, std::array<double, 3>> &predicateCosts)
    : MonitorInterface(formulas, hydra_2_mtl, false, formulaEvaluationMode, domains), predicateCosts(predicateCosts) {
    // Check if formulas are compatible with the monitor
    for (const auto &formula : this->formulas) {
        // If we are running in initial_step mode, we need to check whether the monitor could handle the formula, if
        // we make it into a safety property by adding an outer Globally operator.
        const auto &globally_formula =
            formulaEvaluationMode != initial_step ? formula : Formula(std::make_shared<Globally>(formula.getExpr()));
        if (!std::any_of(supportedFormulaTypes.at(hydra_2_mtl).begin(), supportedFormulaTypes.at(hydra_2_mtl).end(),
                         [globally_formula](const MonitoringProperties formulaType) {
                             return globally_formula.checkCompatibility(formulaType);
                         })) {
            throw std::logic_error("Formula is not supported by monitor: " + formula.to_string());
        }
    }
    createMonitor(formulas, connectedEval, reordering, shortCircuiting);
}

std::map<std::string, std::array<double, 3>> &Hydra2::getPredicateCost() const { return predicateCosts; }

void Hydra2::evalCompleteTrace(std::map<std::string, std::vector<Verdict>> &result,
                               const std::vector<std::string> &formulaIDs) const {
    std::vector<std::shared_ptr<Hydra2Monitor>> relMonitors;
    std::vector<std::string> relFormulaIds;
    std::vector<std::vector<Verdict> *> verdictRefs;
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        if (const auto &formulaId{formulaIds.at(idx)};
            result.find(formulaId) == result.end() and
            (formulaIDs.empty() or std::find(formulaIDs.begin(), formulaIDs.end(), formulaId) != formulaIDs.end())) {
            auto &verdicts = result[formulaId];
            verdicts.reserve(traceLength);
            relMonitors.push_back(monitors.at(idx));
            relFormulaIds.push_back(formulaId);
            verdictRefs.push_back(&verdicts);
        }

    for (size_t i{0}; i < traceLength; ++i) {
        for (size_t m{0}; m < relMonitors.size(); ++m) {
            const auto &mon = relMonitors[m];
            spdlog::debug("Monitor evaluation: " + std::to_string(i) + " - " + relFormulaIds[m]);
            mon->addDomainValues({}); // triggers adding of domain values, e.g., to predicates
            verdictRefs[m]->emplace_back(mon->step(0, i));
        }
        pruneVerdicts(relMonitors, i);
    }
}

void Hydra2::pruneVerdicts(const std::vector<std::shared_ptr<Hydra2Monitor>> &relMonitors,
                           const size_t stepNumber) const {
    for (const auto &mon : relMonitors)
        mon->pruneVerdictHistory(stepNumber, timeStamps.at(stepNumber));
}

size_t Hydra2::getNumStoredVerdicts() const {
    std::set<const Hydra2Monitor *> visited;
    size_t numVerdicts{0};
    for (const auto &mon : monitors)
        numVerdicts += mon->getNumStoredVerdicts(visited);
    return numVerdicts;
}

size_t Hydra2::getNumEvaluations() const {
    std::set<const Hydra2Monitor *> visited;
    size_t evaluations{0};
    for (const auto &mon : monitors)
        evaluations += mon->getNumEvaluations(visited);
    return evaluations;
}

std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>>
Hydra2::getViolatingDomainValues() const {
    std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>> violations;
    for (size_t idx{0}; idx < monitors.size(); ++idx)
        violations[formulaIds.at(idx)] = monitors.at(idx)->getViolatingDomainValues();
    return violations;
}

TreeVis Hydra2::to_tree() const {
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
