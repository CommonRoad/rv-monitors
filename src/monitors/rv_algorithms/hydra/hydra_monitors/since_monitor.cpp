#include "monitors/rv_algorithms/hydra/hydra_monitors/since_monitor.h"

Verdict SinceMonitorHydra::concreteStep() {
    subVerdictF = subMonitorF->concreteStep();
    if (subVerdictF->timeStamp)
        positionMonitorF.update(subVerdictF->timeStamp.value());
    if (!subVerdictF->value)
        last_viol = std::make_pair(positionMonitorF.ts, positionMonitorF.tp);

    if (positionMonitorG.tp == -1) {
        subVerdictG = subMonitorG->concreteStep();
        if (subVerdictG->timeStamp)
            positionMonitorG.update(subVerdictG->timeStamp.value());
    }

    while (!positionMonitorG.eof && positionMonitorG.tp <= positionMonitorF.tp &&
           positionMonitorF.ts - positionMonitorG.ts >= interval.getStart()) {
        if (subVerdictG->value)
            last_sat = std::make_pair(positionMonitorG.ts, positionMonitorG.tp);

        subVerdictG = subMonitorG->concreteStep();
        if (subVerdictG->timeStamp)
            positionMonitorG.update(subVerdictG->timeStamp.value());
        else {
            positionMonitorG.eof = true;
            positionMonitorG.tp++;
        }
    }

    int verd = static_cast<int>(last_sat &&
                                (!interval.isFinite() or positionMonitorF.ts - last_sat->first <= interval.getEnd()) &&
                                (!last_viol || last_sat->second >= last_viol->second));
    return {positionMonitorF.ts, verd != 0};
}

void SinceMonitorHydra::setTrace(const std::shared_ptr<Trace> &tra) {
    subMonitorF->setTrace(tra);
    subMonitorG->setTrace(tra);
}

void SinceMonitorHydra::reset() {
    subMonitorG->reset();
    subMonitorF->reset();
    subVerdictF = {};
    subVerdictG = {};
    last_sat = {};
    last_viol = {};
    positionMonitorG.reset();
    positionMonitorF.reset();
}

std::string SinceMonitorHydra::to_string() {
    std::string itv{" "};
    std::string end{};
    if (!interval.isEmpty()) {
        if (!interval.isFinite())
            end = "inf";
        else
            end = std::to_string(interval.getEnd());
        itv = "[" + std::to_string(interval.getStart()) + ", " + end + "] ";
    }
    return "(" + subMonitorF->to_string() + " S" + itv + subMonitorG->to_string() + ")";
}
