#include <nanobind/nanobind.h>
#include <nanobind/stl/array.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>

#include <monitors/collections/domain.h>

#include <monitors/rv_algorithms/monitor_interface.h>

#include <monitors/rv_algorithms/btt_fsm/btt_fsm.h>
#include <monitors/rv_algorithms/dynamic_programming/dynamic.h>
#include <monitors/rv_algorithms/hydra/hydra.h>
#include <monitors/rv_algorithms/hydra_2/hydra.h>
#include <monitors/rv_algorithms/ltl_event_consuming/event_consuming.h>
#include <monitors/rv_algorithms/mt_fsm/mt_fsm.h>
#include <monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming.h>

#include "monitors.h"

namespace nb = nanobind;

void init_monitors(nb::module_ &m) {
    nb::enum_<FormulaEvaluationMode>(m, "FormulaEvaluationMode")
        .value("globally", globally)
        .value("globally_early_termination", globally_early_termination)
        .value("initial_step", initial_step);

    nb::class_<MonitorInterface>(m, "MonitorInterface")
        .def("__str__", &MonitorInterface::to_string)
        .def("evaluate", &MonitorInterface::evaluate, nb::arg("trace"),
             nb::arg("domains") = std::vector<std::shared_ptr<DomainStorage>>{},
             nb::arg("formulaIDs") = std::vector<std::string>{});

    nb::class_<Hydra, MonitorInterface>(m, "Hydra").def(nb::init<std::vector<Formula>>(), nb::arg("formulas"));

    nb::class_<TreeVis>(m, "TreeVis")
        .def_prop_ro("tree", &TreeVis::getTree)
        .def_prop_ro("nodeTypes", &TreeVis::getNodeTypes)
        .def_prop_ro("rootIDs", &TreeVis::getRootIDs);

    nb::class_<Hydra2, MonitorInterface>(m, "Hydra2")
        .def(nb::init<std::vector<Formula>, bool, bool, bool, FormulaEvaluationMode,
                      std::vector<std::shared_ptr<DomainStorage>>, std::map<std::string, std::array<double, 3>>>(),
             nb::arg("formulas"), nb::arg("connectedEval") = true, nb::arg("reordering") = true,
             nb::arg("shortCircuiting") = true, nb::arg("evaluationMode") = globally,
             nb::arg("domains") = std::vector<std::shared_ptr<DomainStorage>>{},
             nb::arg("predicateCosts") = std::map<std::string, std::array<double, 3>>{})
        .def("to_tree", &Hydra2::to_tree)
        .def("get_num_stored_verdicts", &Hydra2::getNumStoredVerdicts)
        .def("get_num_evaluations", &Hydra2::getNumEvaluations);

    nb::class_<EventConsumingLTL, MonitorInterface>(m, "EventConsumingLTL")
        .def(nb::init<std::vector<Formula>, bool, bool>(), nb::arg("formulas"), nb::arg("pastEvaluation") = false,
             nb::arg("formulaCompatibilityCheck") = true);

    nb::class_<DynamicProgrammingLTL, MonitorInterface>(m, "DynamicProgrammingLTL")
        .def(nb::init<std::vector<Formula>, bool>(), nb::arg("formulas"), nb::arg("pastEvaluation") = false);

    nb::class_<BttFsm, MonitorInterface>(m, "BttFsm")
        .def(nb::init<std::vector<Formula>, bool>(), nb::arg("formulas"), nb::arg("pastEvaluation") = false);

    nb::class_<MtFsm, MonitorInterface>(m, "MtFsm")
        .def(nb::init<std::vector<Formula>, bool>(), nb::arg("formulas"), nb::arg("pastEvaluation") = false);

    nb::class_<MTLEventConsuming, MonitorInterface>(m, "MTLEventConsuming")
        .def(nb::init<std::vector<Formula>>(), nb::arg("formulas"));
}
