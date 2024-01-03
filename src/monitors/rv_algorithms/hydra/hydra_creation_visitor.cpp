#include "monitors/rv_algorithms/hydra/hydra_creation_visitor.h"
#include "monitors/expression_tree/atomic/constant.h"
#include "monitors/expression_tree/atomic/predicate.h"
#include "monitors/expression_tree/binary/temporal/since.h"
#include "monitors/expression_tree/binary/temporal/until.h"
#include "monitors/expression_tree/nary/bool/and.h"
#include "monitors/expression_tree/nary/bool/or.h"
#include "monitors/expression_tree/unary/bool/not.h"
#include "monitors/expression_tree/unary/temporal/next_strong.h"
#include "monitors/expression_tree/unary/temporal/previously.h"
#include "monitors/formula/formula.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/and_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/atomic_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/constant_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/negation_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/next_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/or_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/predicate_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/previous_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/since_monitor.h"
#include "monitors/rv_algorithms/hydra/hydra_monitors/until_monitor.h"

std::shared_ptr<HydraMonitor> HydraCreationVisitor::createMonitor(Formula &formula) {
    auto newMonitor{formula.getExpr()->accept(*this)};
    newMonitor->setFormulaId(formula.getId());
    return newMonitor;
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAtomic(Atomic &top) {
    return std::make_shared<AtomicMonitorHydra>(Formula(std::make_shared<Atomic>(top)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitConstant(Constant &top) {
    return std::make_shared<ConstantMonitorHydra>(Formula(std::make_shared<Constant>(top)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPredicate(Predicate &top) {
    return std::make_shared<PredicateMonitorHydra>(Formula(std::make_shared<Predicate>(top)));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitPreviously(Previously &top) {
    return std::make_shared<PreviousMonitorHydra>(Formula(std::make_shared<Previously>(top)),
                                                  top.getLeft()->accept(*this),
                                                  IntervalTemporalOperator(top.getInterval()));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNext(NextStrong &top) {
    return std::make_shared<NextMonitorHydra>(Formula(std::make_shared<NextStrong>(top)), top.getLeft()->accept(*this),
                                              IntervalTemporalOperator(top.getInterval()));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitSince(Since &top) {
    return std::make_shared<SinceMonitorHydra>(Formula(std::make_shared<Since>(top)), top.getLeft()->accept(*this),
                                               top.getRight()->accept(*this),
                                               IntervalTemporalOperator(top.getInterval()));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitUntil(Until &top) {
    return std::make_shared<UntilMonitorHydra>(Formula(std::make_shared<Until>(top)), top.getLeft()->accept(*this),
                                               top.getRight()->accept(*this),
                                               IntervalTemporalOperator(top.getInterval()));
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitOr(Or &top) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    for (const auto &expr : top.getExpressions())
        monitors.push_back(expr->accept(*this));
    return std::make_shared<OrMonitorHydra>(Formula(std::make_shared<Or>(top)), monitors);
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitAnd(And &top) {
    std::vector<std::shared_ptr<HydraMonitor>> monitors;
    for (const auto &expr : top.getExpressions())
        monitors.push_back(expr->accept(*this));
    return std::make_shared<AndMonitorHydra>(Formula(std::make_shared<And>(top)), monitors);
}

std::shared_ptr<HydraMonitor> HydraCreationVisitor::visitNot(Not &top) {
    return std::make_shared<NegationMonitorHydra>(Formula(std::make_shared<Not>(top)), top.getLeft()->accept(*this));
}