//
// Created by Sebastian Maierhofer and Evald Nexhipi.
// Technical University of Munich - Cyber-Physical Systems Group
// Copyright (c) 2021 Technical University of Munich. All rights reserved.
// Credits: BMW Car@TUM
//
#include "mtl_event_consuming.h"
#include "../../rewriting/mtl_rewrite_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "mtl_event_consuming_visitor.h"
#include <iostream>
#include <utility>

MTLEventConsuming::MTLEventConsuming(const std::vector<Formula> &formulas) : MonitorInterface((std::move(formulas))) {}
// MTLEventConsuming::MTLEventConsuming(const Formula &formula) : MonitorInterface((formula)) {}

std::map<std::string, std::vector<Verdict>> MTLEventConsuming::evaluate(Trace &trace) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace.getTrace().empty()) {
        std::cout << "Trace is empty" << std::endl;
        result[formulas.at(0).getId()] = {Verdict{false}};
        return result;
    }
    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
    MtlRewriteVisitor rw;
    MTLEventConsumingVisitor ev;

    // Check global satisfiability of the formula
    if (auto globalExpr = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr())) {
        if (globalExpr->getInterval()->isFinite())
            formulaCopy.setExpr(std::make_shared<Globally>(formulaCopy.getExpr()));
    }
    else {
        formulaCopy.setExpr(std::make_shared<Globally>(formulaCopy.getExpr()));
    }
    rw.rewrite(formulaCopy);

    for (size_t i{0}; i < trace.getLength(); ++i) {
        ev.consumeEvent(formulaCopy, trace, i);
        rw.rewrite(formulaCopy);
        if (formulaCopy.to_string() == "false") {
            break;
        }
        if (formulaCopy.to_string() == "true") {
            break;
        }
    }

    result[formulas.at(0).getId()] = {Verdict{formulaCopy.to_string() == "true"}};
    return result;
}
std::string MTLEventConsuming::to_string() const { return "MTL Event Consuming"; }