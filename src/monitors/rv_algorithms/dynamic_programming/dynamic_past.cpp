#include "dynamic_past.h"
#include "dynamic.h"
#include "monitors/expression_tree/unary/temporal/ptlf/once.h"
#include "monitors/expression_tree/unary/temporal/ptlf/previously.h"
#include "monitors/utils/utils.h"
#include <algorithm>
#include <iostream>
#include <queue>

std::vector<bool> DynamicPast::initializePre(Event &event,
                                             const std::vector<std::shared_ptr<Expression>> &subformulas) {
    std::vector<bool> pre;
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();
    pre.resize(subformulas.size());
    // Fill pre backwards
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        Dynamic::initialize(j, pre, eventMap, subformulas);
        // Previously
        if (std::dynamic_pointer_cast<Previously>(subformulas[j])) {
            pre[j] = true;
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = std::distance(subformulas.begin(), itr);
            pre[j] = pre[indexLeft];
        }
    }

    return pre;
}

std::vector<bool> DynamicPast::updateNow(Event &event, const std::vector<std::shared_ptr<Expression>> &subformulas,
                                         const std::vector<bool> &pre) {
    std::vector<bool> now;
    now.resize(pre.size());
    std::map<std::string, lazy_bool> eventMap = event.getEventMap();

    // j >= 0 && j < subformulas.size() to avoid underflow
    for (size_t j = subformulas.size() - 1; j >= 0 && j < subformulas.size(); --j) {
        Dynamic::update(j, now, pre, eventMap, subformulas);
        // Previously
        if (auto previously = std::dynamic_pointer_cast<Previously>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), previously->getLeft());
            auto indexLeft = std::distance(subformulas.begin(), itr);
            now[j] = pre[indexLeft];
        }
        // Once
        else if (auto once = std::dynamic_pointer_cast<Once>(subformulas[j])) {
            auto itr = std::find(subformulas.begin(), subformulas.end(), once->getLeft());
            auto indexLeft = std::distance(subformulas.begin(), itr);
            now[j] = now[indexLeft] or pre[j];
        }
    }
    return now;
}

bool DynamicPast::checkTraceDynamically(const Formula &formula, Trace &trace) {
    // Empty trace
    if (trace.getTrace().empty()) {
        // std::cout << "Trace is empty" << std::endl;
        return false;
    }
    std::vector<std::shared_ptr<Expression>> subformulae = Utils::subformulasBreadthFirstOrder(formula);
    std::vector<bool> pre = initializePre(trace.getTraceRef()[0], subformulae);
    for (size_t i = 1; i < trace.getTrace().size(); ++i) {
        pre = updateNow(trace.getTraceRef()[i], subformulae, pre);
        // Only for safety properties!
        if (!pre[0]) {
            // std::cout << "Formula violated in event " << i - 1 << std::endl;
            return false;
        }
    }
    return pre[0];
}