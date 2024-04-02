#include "monitors/rv_algorithms/monitor_interface.h"

#include <utility>

MonitorInterface::MonitorInterface(Formula formula, AlgorithmType algorithm, bool formulaCompatibilityCheck,
                                   bool earlyTermination, const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : formulas({std::move(formula)}), algorithm(algorithm), earlyTermination(earlyTermination), domains(domains) {
    if (formulaCompatibilityCheck and
        !std::any_of(supportedFormulaTypes.at(this->algorithm).begin(), supportedFormulaTypes.at(this->algorithm).end(),
                     [formula](MonitoringProperties formulaType) { return formula.checkCompatibility(formulaType); }))
        throw std::logic_error("Formula is not supported by monitor!");
}

MonitorInterface::MonitorInterface(std::vector<Formula> formulas, AlgorithmType algorithm,
                                   bool formulaCompatibilityCheck, bool earlyTermination,
                                   const std::vector<std::shared_ptr<DomainStorage>> &domains)
    : formulas(std::move(formulas)), algorithm(algorithm), earlyTermination(earlyTermination), domains(domains) {
    if (formulaCompatibilityCheck)
        for (const auto &formula : this->formulas)
            if (!std::any_of(
                    supportedFormulaTypes.at(this->algorithm).begin(), supportedFormulaTypes.at(this->algorithm).end(),
                    [formula](MonitoringProperties formulaType) { return formula.checkCompatibility(formulaType); }))
                throw std::logic_error("Formula is not supported by monitor: " + formula.to_string());
}

const std::vector<Formula> &MonitorInterface::getFormulas() const { return formulas; }

std::vector<std::shared_ptr<DomainStorage>> &MonitorInterface::getDomains() { return domains; }

std::map<std::string, std::map<size_t, std::vector<std::vector<DomainValue>>>>
MonitorInterface::getViolatingDomainValues() const {
    throw std::runtime_error("MonitorInterface::getViolatingDomainValues only implemented for Hydra2.");
}

const std::map<AlgorithmType, std::vector<MonitoringProperties>> supportedFormulaTypes{
    {AlgorithmType::hydra_mtl, {{FormulaType::safetyGeneral, true}}},
    {AlgorithmType::hydra_2_mtl, {{FormulaType::safetyFirstOrder, true}}},
    {AlgorithmType::btt_fsm, {{FormulaType::futureOnly, false}, {FormulaType::pastSafety, false}}},
    {AlgorithmType::dynamic_programming_ltl, {{FormulaType::futureOnly, false}, {FormulaType::pastSafety, false}}},
    {AlgorithmType::event_consuming_ltl, {{FormulaType::futureOnly, false}, {FormulaType::pastSafety, false}}},
    {AlgorithmType::mt_fsm, {{FormulaType::futureOnly, false}, {FormulaType::pastSafety, false}}},
    {AlgorithmType::event_consuming_mtl, {{FormulaType::safetyGeneral, true, true}}},
};
