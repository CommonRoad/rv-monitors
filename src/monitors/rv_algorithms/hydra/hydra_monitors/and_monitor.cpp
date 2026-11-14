#include "monitors/rv_algorithms/hydra/hydra_monitors/and_monitor.h"

Verdict AndMonitorHydra::concreteStep() {
    Verdict verdict{subMonitors.at(0)->concreteStep()};
    for (size_t i{1}; i < subMonitors.size(); ++i)
        verdict = verdict && subMonitors.at(i)->concreteStep();
    return verdict;
}

void AndMonitorHydra::setTrace(const std::shared_ptr<Trace> &tra) {
    for (const auto &mon : subMonitors)
        mon->setTrace(tra);
}
void AndMonitorHydra::reset() {
    for (const auto &mon : subMonitors)
        mon->reset();
}

std::string AndMonitorHydra::to_string() {
    std::string str{"("};
    for (const auto &mon : subMonitors)
        str += mon->to_string() + " & ";
    str.erase(str.size() - 3);
    str += ")";
    return str;
}
