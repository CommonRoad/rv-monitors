#include <any>
#include <cstdint>
#include <functional>
#include <stdexcept>
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <utility>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/tuple.h>
#include <nanobind/stl/vector.h>

#include <nanobind/make_iterator.h>

#include <monitors/collections/domain.h>
#include <monitors/collections/domain_collection.h>
#include <monitors/collections/domain_value.h>
#include <monitors/collections/function.h>

#include "domain.h"

namespace nb = nanobind;

static nb::object tryConvert(const DomainValue &domainValue) {
    if (auto ts = domainValue.getTimeStep(); ts.has_value()) {
        return nb::int_(ts.value());
    }

    try {
        auto obj = std::any_cast<nb::object>(domainValue.getValue());

        if (!obj.is_valid()) {
            throw std::runtime_error{"failed to recover Python object from DomainValue"};
        }

        return obj;
    } catch (const std::bad_any_cast &bac) {
        // We only expect time_step/size_t or nb::object in DomainValue
        throw std::runtime_error{"unexpected DomainValue contents"};
    }
}

static void init_functions(nb::module_ &m) {
    nb::class_<IFunction<bool>>(m, "IBoolFunction")
        .def_prop_ro("name", &IFunction<bool>::getName)
        .def_prop_ro("argument_names", &IFunction<bool>::getArgumentNames)
        .def_prop_ro("costs", &IFunction<bool>::getCosts)
        .def("get_function", nb::overload_cast<>(&IFunction<bool>::getFunction))
        .def("get_function", nb::overload_cast<const std::vector<DomainValue> &>(&IFunction<bool>::getFunction),
             nb::arg("args"));

    {
        using arg_type = nb::object;

        using function_type = Function<bool, arg_type>;
        nb::class_<function_type, IFunction<bool>>(m, "Function1")
            .def(
                "__init__",
                [](function_type *v, std::function<bool(arg_type)> function, std::string name,
                   std::vector<std::string> argumentNames, float costs) {
                    if (argumentNames.size() != 1) {
                        throw std::invalid_argument{"invalid number of argument names"};
                    }

                    auto arg_conversion = [](const std::vector<DomainValue> &values) -> std::tuple<arg_type> {
                        if (values.size() != 1) {
                            throw std::invalid_argument{"invalid number of arguments"};
                        }

                        auto obj = tryConvert(values.front());

                        return std::make_tuple(obj);
                    };

                    new (v)
                        function_type(std::move(name), std::move(argumentNames), std::move(function), arg_conversion);
                },
                nb::arg("function"), nb::arg("name"), nb::arg("argument_names"), nb::arg("costs") = 0.0,
                nb::sig("def __init__(self, function: Callable[[Any], bool], name: str, argument_names: Sequence[str], "
                        "costs: float = 0.0) -> None"))
            .def("evaluate", &function_type::evaluateFunction)
            .def("__call__", &function_type::evaluateFunction);
    }

    {
        using function_type = Function<bool, nb::object, nb::object>;
        nb::class_<function_type, IFunction<bool>>(m, "Function2")
            .def(
                "__init__",
                [](function_type *v, std::function<bool(nb::object, nb::object)> function, std::string name,
                   std::vector<std::string> argumentNames, float costs) {
                    if (argumentNames.size() != 2) {
                        throw std::invalid_argument{"invalid number of argument names"};
                    }

                    auto arg_conversion =
                        [](const std::vector<DomainValue> &values) -> std::tuple<nb::object, nb::object> {
                        if (values.size() != 2) {
                            throw std::invalid_argument{"invalid number of arguments"};
                        }

                        return std::make_tuple(tryConvert(values[0]), tryConvert(values[1]));
                    };

                    new (v)
                        function_type(std::move(name), std::move(argumentNames), std::move(function), arg_conversion);
                },
                nb::arg("function"), nb::arg("name"), nb::arg("argument_names"), nb::arg("costs") = 0.0,
                nb::sig("def __init__(self, function: Callable[[Any], Any, bool], name: str, argument_names: "
                        "Sequence[str], costs: float = 0.0) -> None"))
            .def("evaluate", &function_type::evaluateFunction)
            .def("__call__", &function_type::evaluateFunction);
    }

    {
        using function_type = Function<bool, nb::object, nb::object, nb::object>;
        nb::class_<function_type, IFunction<bool>>(m, "Function3")
            .def(
                "__init__",
                [](function_type *v, std::function<bool(nb::object, nb::object, nb::object)> function, std::string name,
                   std::vector<std::string> argumentNames, float costs) {
                    if (argumentNames.size() != 3) {
                        throw std::invalid_argument{"invalid number of argument names"};
                    }

                    auto arg_conversion =
                        [](const std::vector<DomainValue> &values) -> std::tuple<nb::object, nb::object, nb::object> {
                        if (values.size() != 3) {
                            throw std::invalid_argument{"invalid number of arguments"};
                        }

                        return std::make_tuple(tryConvert(values[0]), tryConvert(values[1]), tryConvert(values[2]));
                    };

                    new (v)
                        function_type(std::move(name), std::move(argumentNames), std::move(function), arg_conversion);
                },
                nb::arg("function"), nb::arg("name"), nb::arg("argument_names"), nb::arg("costs") = 0.0,
                nb::sig("def __init__(self, function: Callable[[Any, Any, Any], bool], name: str, argument_names: "
                        "Sequence[str], costs: float = 0.0) -> None"))
            .def("evaluate", &function_type::evaluateFunction)
            .def("__call__", &function_type::evaluateFunction);
    }

    {
        using function_type = Function<bool, nb::object, nb::object, nb::object, nb::object>;
        nb::class_<function_type, IFunction<bool>>(m, "Function4")
            .def(
                "__init__",
                [](function_type *v, std::function<bool(nb::object, nb::object, nb::object, nb::object)> function,
                   std::string name, std::vector<std::string> argumentNames, float costs) {
                    if (argumentNames.size() != 4) {
                        throw std::invalid_argument{"invalid number of argument names"};
                    }

                    auto arg_conversion = [](const std::vector<DomainValue> &values)
                        -> std::tuple<nb::object, nb::object, nb::object, nb::object> {
                        if (values.size() != 4) {
                            throw std::invalid_argument{"invalid number of arguments"};
                        }

                        return std::make_tuple(tryConvert(values[0]), tryConvert(values[1]), tryConvert(values[2]),
                                               tryConvert(values[3]));
                    };

                    new (v)
                        function_type(std::move(name), std::move(argumentNames), std::move(function), arg_conversion);
                },
                nb::arg("function"), nb::arg("name"), nb::arg("argument_names"), nb::arg("costs") = 0.0,
                nb::sig("def __init__(self, function: Callable[[Any, Any, Any, Any], bool], name: str, argument_names: "
                        "Sequence[str], costs: float = 0.0) -> None"))
            .def("evaluate", &function_type::evaluateFunction)
            .def("__call__", &function_type::evaluateFunction);
    }

    {
        using function_type = Function<bool, nb::object, nb::object, nb::object, nb::object, nb::object>;
        nb::class_<function_type, IFunction<bool>>(m, "Function5")
            .def(
                "__init__",
                [](function_type *v,
                   std::function<bool(nb::object, nb::object, nb::object, nb::object, nb::object)> function,
                   std::string name, std::vector<std::string> argumentNames, float costs) {
                    if (argumentNames.size() != 5) {
                        throw std::invalid_argument{"invalid number of argument names"};
                    }

                    auto arg_conversion = [](const std::vector<DomainValue> &values)
                        -> std::tuple<nb::object, nb::object, nb::object, nb::object, nb::object> {
                        if (values.size() != 5) {
                            throw std::invalid_argument{"invalid number of arguments"};
                        }

                        return std::make_tuple(tryConvert(values[0]), tryConvert(values[1]), tryConvert(values[2]),
                                               tryConvert(values[3]), tryConvert(values[4]));
                    };

                    new (v)
                        function_type(std::move(name), std::move(argumentNames), std::move(function), arg_conversion);
                },
                nb::arg("function"), nb::arg("name"), nb::arg("argument_names"), nb::arg("costs") = 0.0,
                nb::sig("def __init__(self, function: Callable[[Any, Any, Any, Any, Any], bool], name: str, "
                        "argument_names: Sequence[str], costs: float = 0.0) -> None"))
            .def("evaluate", &function_type::evaluateFunction)
            .def("__call__", &function_type::evaluateFunction);
    }
}

static std::vector<std::any> convertAny(const std::vector<nb::object> &objects) {
    std::vector<std::any> values;
    values.reserve(objects.size());
    for (const auto &obj : objects) {
        values.emplace_back(obj);
    }
    return values;
}

static std::vector<std::any> convertAny(const nb::list &list) {
    std::vector<std::any> values;
    values.reserve(list.size());
    for (const auto &handle : list) {
        auto obj = nb::borrow(handle);
        values.emplace_back(obj);
    }
    return values;
}

// Create an iterator that casts std::any to nb::object on access
template <nb::rv_policy Policy = nb::rv_policy::reference_internal, typename... Extra>
static auto make_any_iterator(nb::handle scope, const char *name, std::vector<std::any>::iterator &&first,
                              std::vector<std::any>::iterator &&last, Extra &&...extra) {
    struct any_iterator_access {
        nb::object operator()(std::vector<std::any>::iterator &it) const { return std::any_cast<nb::object>(*it); }
    };

    // NOTE: Technically this is an internal nanobind function
    // Check <nanobind/make_iterator.h> in case it ever breaks
    return nb::detail::make_iterator_impl<any_iterator_access, Policy, std::vector<std::any>::iterator,
                                          std::vector<std::any>::iterator, nb::object, Extra...>(
        scope, name, std::forward<std::vector<std::any>::iterator>(first),
        std::forward<std::vector<std::any>::iterator>(last), std::forward<Extra>(extra)...);
}

template <typename T, typename F>
static inline std::pair<const std::type_index, std::function<nb::object(std::any const &)>> to_any_visitor(const F &f) {
    return {std::type_index(typeid(T)), [g = f](std::any const &a) { return g(std::any_cast<T const &>(a)); }};
}

template <typename T>
static inline std::pair<const std::type_index, std::function<nb::object(std::any const &)>> cast_any_visitor() {
    return to_any_visitor<T>([](const T &t) { return nb::cast(t); });
}

void init_domain(nb::module_ &m) {
    // Construct value ID based on Python string conversion (same as calling str(obj) in Python)
    auto objStrIdFunction = [](std::any value) -> std::string {
        auto obj = std::any_cast<nb::object>(value);
        auto str = nb::str(obj);
        return std::string{str.c_str()};
    };

    nb::class_<ConstantValue>(m, "ConstantValue")
        .def("__init__",
             [idFunction = objStrIdFunction](ConstantValue *v, std::string name, nb::any &value) {
                 new (v) ConstantValue(std::move(name), dynamic_cast<const nb::object &>(value), idFunction);
             })
        .def_prop_ro("name", &ConstantValue::getName);

    nb::class_<ConstantCollection>(m, "ConstantCollection").def(nb::init<std::vector<ConstantValue>>());

    nb::class_<DomainValue>(m, "DomainValue")
        .def("__init__",
             [idFunction = objStrIdFunction](DomainValue *v, std::string name, const nb::any &value) {
                 // TODO: Is constVar = true always correct here?
                 new (v) DomainValue(std::move(name), dynamic_cast<const nb::object &>(value), true, idFunction);
             })
        .def_prop_ro("variable", &DomainValue::getVariable)
        .def_prop_ro("value", [](const DomainValue &dv) -> nb::object {
            static std::unordered_map<std::type_index, std::function<nb::object(std::any const &)>> any_visitor{
                to_any_visitor<nb::object>([](const nb::object &obj) { return obj; }),
                to_any_visitor<nb::any>([](const nb::any &obj) { return dynamic_cast<const nb::object &>(obj); }),
                cast_any_visitor<bool>(),
                cast_any_visitor<int8_t>(),
                cast_any_visitor<uint8_t>(),
                cast_any_visitor<int16_t>(),
                cast_any_visitor<uint16_t>(),
                cast_any_visitor<int32_t>(),
                cast_any_visitor<uint32_t>(),
                cast_any_visitor<int64_t>(),
                cast_any_visitor<uint64_t>(),
                cast_any_visitor<float>(),
                cast_any_visitor<double>(),
                cast_any_visitor<std::string>(),
            };

            const auto &val = dv.getValue();

            if (const auto it = any_visitor.find(std::type_index(val.type())); it != any_visitor.cend()) {
                return it->second(val);
            } else {
                nb::raise_type_error("failed to convert DomainValue contents");
            }
        });

    auto m_function = m.def_submodule("_function");
    init_functions(m_function);

    nb::class_<DomainStorage>(m, "DomainStorage", nb::sig("class DomainStorage(typing.Sized)"))
        .def(nb::init<std::string>(), nb::arg("name"))
        .def(
            "__init__",
            [idFunction = objStrIdFunction](DomainStorage *ds, std::string name, const nb::list &objects) {
                new (ds) DomainStorage(std::move(name), convertAny(objects), idFunction);
            },
            nb::arg("name"), nb::arg("values"))
        .def(
            "__init__",
            [idFunction = objStrIdFunction](DomainStorage *ds, std::string name,
                                            const std::function<nb::list(size_t)> &dynamicDomainStorageFunction) {
                auto convertedFunction = [ddsf = dynamicDomainStorageFunction](size_t index) -> std::vector<std::any> {
                    auto valueList = ddsf(index);

                    return convertAny(valueList);
                };

                auto alwaysValid = [](size_t timeStep, std::any value) { return true; };
                new (ds) DomainStorage(std::move(name), convertedFunction, idFunction, alwaysValid);
            },
            nb::arg("name"), nb::arg("dynamic_domain_storage_function"))
        .def("__len__", &DomainStorage::size)
        .def_prop_ro("name", &DomainStorage::getName)
        .def(
            "values",
            [](DomainStorage &ds) {
                return make_any_iterator(nb::type<DomainStorage>(), "values_iterator", ds.begin(), ds.end());
            },
            nb::keep_alive<0, 1>())
        .def("reset", &DomainStorage::reset)
        .def("update_dynamic_domain", [](DomainStorage &ds, size_t stepNumber) { ds.updateDynamicDomain(stepNumber); })
        .def("is_empty", &DomainStorage::empty)
        .def("__bool__", [](const DomainStorage &ds) { return !ds.empty(); })
        .def(
            "__iter__",
            [](DomainStorage &ds) {
                return make_any_iterator(nb::type<DomainStorage>(), "values_iterator", ds.begin(), ds.end());
            },
            nb::keep_alive<0, 1>());

    nb::class_<DomainCollection>(m, "DomainCollection", nb::sig("class DomainCollection(typing.Sized)"))
        .def(nb::init<const DomainStorage &>())
        .def(nb::init<const std::vector<DomainStorage> &>())
        .def("__len__", &DomainCollection::size)
        .def("add_domain", &DomainCollection::addDomain)
        .def("remove_domain", &DomainCollection::removeDomain)
        .def("__delitem__", &DomainCollection::removeDomain)
        .def("get_domain", &DomainCollection::getDomain)
        .def("__getitem__", &DomainCollection::getDomain);
}
