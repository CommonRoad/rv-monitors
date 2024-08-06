#include <functional>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/operators.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/set.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/unordered_map.h>
#include <nanobind/stl/vector.h>
#include <pyerrors.h>

#include <monitors/collections/constant_collection.h>
#include <monitors/collections/function.h>
#include <monitors/formula/formula.h>
#include <monitors/parser/parser.h>
#include <monitors/trace/event.h>
#include <monitors/trace/event_value.h>
#include <monitors/utils/utils.h>
#include <monitors/utils/verdict.h>

#include "domain.h"
#include "monitors.h"

namespace nb = nanobind;

NB_MODULE(_rvm, m) {
    nb::class_<Formula>(m, "Formula")
        .def("__str__", &Formula::to_string)
        .def_prop_rw("id", &Formula::getId, &Formula::setId)
        .def("to_nnf", &Formula::toNnf);

    nb::class_<EventValue<bool>>(m, "BoolEventValue")
        .def(nb::init<bool>())
        .def(nb::init<std::function<bool()>>())
        .def("__bool__", &EventValue<bool>::get)
        .def("__str__", &EventValue<bool>::to_string);

    nb::class_<Event>(m, "Event")
        .def(nb::init<size_t, bool>())
        .def(nb::init<event_map_t, size_t, bool>())
        .def("__str__", &Event::to_string)
        .def_prop_ro("is_terminal", &Event::isTerminal)
        .def_prop_ro("time_step", &Event::getTimeStep)
        .def_prop_ro("event_map", &Event::getEventMap)
        .def("get_value", [](const Event &e, const std::string &atomic) { return e.getValue(atomic); })
        .def("__getitem__", [](const Event &e, const std::string &atomic) { return e.getValue(atomic); });

    init_domain(m);

    nb::class_<Trace>(m, "Trace", nb::sig("class Trace(Sequence[Event])"))
        .def(nb::init<>())
        .def(
            "__init__",
            [](Trace *t, std::vector<Event> trace, std::vector<std::shared_ptr<IFunction<bool>>> functionCollection,
               ConstantCollection constantCollection) {
                new (t) Trace(std::move(trace), std::move(functionCollection), std::move(constantCollection));
            },
            nb::arg("trace"), nb::arg("function_collection") = {}, nb::arg("constantCollection") = ConstantCollection())
        .def_prop_ro("length", &Trace::getLength)
        .def("__len__", &Trace::getLength)
        .def("get_event", &Trace::getEvent)
        .def("__getitem__", [](const std::shared_ptr<Trace> &trace, size_t index) { return trace->getEvent(index); })
        .def("__getitem__", [](const std::shared_ptr<Trace> &trace, nb::slice _slice) -> nb::sequence {
            nb::raise("Trace slicing is not implemented");
        });

    nb::class_<Verdict>(m, "Verdict")
        .def(nb::init<bool>(), nb::arg("value"))
        .def(nb::init<size_t, bool>(), nb::arg("time_stamp"), nb::arg("value"))
        .def("__bool__", [](const Verdict &v) { return v.value; })
        .def_rw("value", &Verdict::value)
        .def_rw("time_stamp", &Verdict::timeStamp)
        .def("__str__", &Verdict::to_string)
        .def(
            "__eq__",
            [](const Verdict &lhs, const nb::object &rhs) -> bool {
                if (nb::isinstance<bool>(rhs)) {
                    bool rhs_val = nb::cast<bool>(rhs, false);
                    if (lhs.timeStamp.has_value()) {
                        PyErr_SetString(PyExc_NotImplementedError, "can't compare timestamped Verdict with bool");
                        throw nb::python_error();
                    }
                    return lhs.value == rhs_val;
                } else if (nb::isinstance<Verdict>(rhs)) {
                    auto rhs_val = nb::cast<Verdict>(rhs, false);
                    return lhs == rhs_val;
                } else {
                    PyErr_SetString(PyExc_NotImplementedError, nullptr);
                    throw nb::python_error();
                }
            },
            nb::is_operator())
        .def(
            "__and__", [](const Verdict &lhs, const Verdict &rhs) { return lhs && rhs; }, nb::is_operator())
        .def(
            "__or__", [](const Verdict &lhs, const Verdict &rhs) { return lhs || rhs; }, nb::is_operator());

    nb::class_<Parser>(m, "Parser")
        .def_static("parse", &Parser::parse, nb::arg("formula"), nb::arg("id") = "")
        .def_static("parse_log", &Parser::parse_log);

    m.def(
        "create_trace",
        [](const std::set<std::string> &atomics, const std::vector<std::vector<std::string>> &trueAtomics,
           const std::vector<size_t> &timeSteps = std::vector<size_t>{}) {
            return Utils::createTrace(atomics, trueAtomics, timeSteps);
        },
        nb::arg("atomics").noconvert() = std::set<std::string>{},
        nb::arg("true_atomics").noconvert() = std::vector<std::vector<std::string>>{},
        nb::arg("time_steps").noconvert() = std::vector<size_t>{});

    init_monitors(m);
}
