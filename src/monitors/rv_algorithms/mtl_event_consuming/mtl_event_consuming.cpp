#include "monitors//rewriting/mtl_rewrite_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/unary/event_operator.h"
#include "monitors/expression_tree/unary/temporal/globally.h"
#include "monitors/rv_algorithms/mtl_event_consuming/mtl_event_consuming_visitor.h"
#include <iostream>

MTLEventConsuming::MTLEventConsuming(const std::vector<Formula> &formulas)
    : MonitorInterface(formulas, AlgorithmType::event_consuming_mtl) {}

std::map<std::string, std::vector<Verdict>>
MTLEventConsuming::evaluate(const std::shared_ptr<Trace> &trace,
                            const std::vector<std::shared_ptr<DomainStorage>> &domains,
                            const std::vector<std::string> &formulaIDs) {
    std::map<std::string, std::vector<Verdict>> result;
    // Empty trace
    if (trace->getTrace().empty()) {
        std::cout << "Hydra2::evaluate | Trace is empty\n";
        result[formulas.at(0).getId()] = {};
        return result;
    }

    // Copy formula before working on it
    auto formulaCopy = formulas.at(0).deepCopy();
    MtlRewriteVisitor rwv;
    MTLEventConsumingVisitor ecv;

    // Check global satisfiability of the formula
    if (auto globalExpr = std::dynamic_pointer_cast<Globally>(formulaCopy.getExpr())) {
        if (globalExpr->getInterval()->isFinite())
            formulaCopy.setExpr(std::make_shared<Globally>(formulaCopy.getExpr()));
    } else {
        formulaCopy.setExpr(std::make_shared<Globally>(formulaCopy.getExpr()));
    }
    rwv.rewrite(formulaCopy);

    for (size_t i{0}; i < trace->getLength(); ++i) {
        ecv.consumeEvent(formulaCopy, trace, i);
        rwv.rewrite(formulaCopy);
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
