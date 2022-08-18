//
// Created by Sebastian Maierhofer.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Sebastian Maierhofer - Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "predicate_monitor.h"
#include <iostream>

Verdict PredicateMonitor::concreteStep(size_t parentId, size_t stepNumber) {
    if (traceIdx[parentId] < trace.getLength()) {
        const Predicate &predicate = *std::dynamic_pointer_cast<Predicate>(formula.getExpr());
        auto event{trace.getEvent(traceIdx[parentId])};
        if (trace.isTraced(predicate, traceIdx[parentId])) {
            return {event.getTimeStep(), event.getValue(predicate)};
        } else {
            std::string predName;
            auto iterFunc{variableValues.find(predicate.getName())};
            if (iterFunc != variableValues.end())
                predName = iterFunc->second.getVal();
            else
                predName = predicate.getName();
            Function function = functions.getFunction(predName);
            int argc = static_cast<int>(predicate.getArguments().size());
            std::vector<std::string> args;
            if (function.getWithTimeStepArgument()) {
                argc++;
                args.push_back(std::to_string(traceIdx[parentId]));
            }
            for (const auto &[argName, argType] : predicate.getArguments()) {
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
                case Predicate::PARAMETER:
                    break;
                }
            }
            std::vector<char *> arg_ptrs;
            for (auto &arg : args)
                arg_ptrs.push_back(arg.data());
            return {event.getTimeStep(), function.getFunction()(argc, arg_ptrs.data())};
        }
    } else
        return {false};
}

void PredicateMonitor::setTrace(const Trace &tra) { trace = tra; }

void PredicateMonitor::setVariableValues(std::map<std::string, Value> values) { variableValues = values; }

void PredicateMonitor::reset(std::optional<size_t> stepNumber) {
    HydraMonitor::reset(stepNumber);
    variableValues.clear();
}

std::string PredicateMonitor::to_string() { return formula.to_string(); }
