//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"
#include <iostream>

Verdict PredicateMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (stepNumber < trace.getLength()) {
        const Predicate &predicate = *std::dynamic_pointer_cast<Predicate>(formula.getExpr());
        auto event{trace.getEvent(traceIdx[parentId])};
        if (trace.predicateIncludedByTrace(predicate, traceIdx[parentId])) {
            return {event.getTimeStep(), event.getValue(predicate)};
        } else {
            return evalPredicateWithQuantifiedVars(parentId, predicate, event);
        }
    } else
        return {false};
}

void PredicateMonitor::setTrace(const Trace &tra) { trace = tra; }

void PredicateMonitor::setVariableValues(std::map<std::string, Value> values) { variableValues = values; }

void PredicateMonitor::reset() {
    Hydra2Monitor::reset();
    variableValues.clear();
}

std::shared_ptr<Hydra2Monitor> PredicateMonitor::clone() { return std::make_shared<PredicateMonitor>(*this); }

std::string PredicateMonitor::to_string() { return formula.to_string(); }

Verdict PredicateMonitor::evalPredicateWithQuantifiedVars(size_t parentId, const Predicate &predicate, const Event &event) {
    std::string predName;
    auto iterFunc{variableValues.find(predicate.getName())};
    if (iterFunc != variableValues.end()) // get predicate name either by the current value if it is a dynamic predicate (A x in D. x(a))
                                          // or as fixed one
        predName = iterFunc->second.getVal();
    else
        predName = predicate.getName();

    Function function = functions.getFunction(predName); // extract function to evaluate predicate
    int argc = static_cast<int>(predicate.getArguments().size());
    std::vector<std::string> args; // arguments of predicate
    if (function.getWithTimeStepArgument()) { // insert current time step if the predicate is time dependent
        argc++;
        args.push_back(std::to_string(traceIdx[parentId]));
    }
    for (const auto &[argName, argType] : predicate.getArguments()) { // insert all predicate arguments which can be constants and current values of variables
        switch (argType) {
        case Predicate::VARIABLE: {
            auto iterVar{variableValues.find(argName)};
            if (iterVar != variableValues.end())
                args.push_back(iterVar->second.getVal());
            else
                throw std::runtime_error("Undefined variable " + argName + "!");
            break;
        }
        case Predicate::CONSTANT: {
            args.push_back(argName);
            break;
        }
        }
    }

    std::vector<char *> arg_ptrs;
    for (auto &arg : args)
        arg_ptrs.push_back(arg.data());
    return {event.getTimeStep(), function.getFunction()(argc, arg_ptrs.data())};
}
